import tkinter as tk
from tkinter import filedialog, messagebox, ttk
import serial.tools.list_ports
import struct
from PIL import Image, ImageTk
import subprocess
import os
import threading

class ImageFlasherApp:
    def __init__(self, root):
        self.root = root
        self.root.title("ESP32 Image Flasher")
        self.root.geometry("600x550")
        
        self.image_path = None
        self.processed_image = None
        self.flash_address = "0x311000"  # Default address
        
        self.create_widgets()
        self.refresh_ports()
    
    def create_widgets(self):
        # Main frame
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.pack(fill="both", expand=True)
        
        # Port selection
        port_frame = ttk.LabelFrame(main_frame, text="Serial Port", padding="5")
        port_frame.pack(fill="x", pady=5)
        
        ttk.Label(port_frame, text="Port:").grid(row=0, column=0, sticky="w", padx=5)
        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(port_frame, textvariable=self.port_var, state="readonly", width=30)
        self.port_combo.grid(row=0, column=1, padx=5, pady=5, sticky="w")
        
        ttk.Button(port_frame, text="Refresh", command=self.refresh_ports).grid(row=0, column=2, padx=5)
        
        # Flash address
        addr_frame = ttk.LabelFrame(main_frame, text="Flash Settings", padding="5")
        addr_frame.pack(fill="x", pady=5)
        
        ttk.Label(addr_frame, text="Flash Address:").grid(row=0, column=0, sticky="w", padx=5)
        self.addr_var = tk.StringVar(value=self.flash_address)
        addr_entry = ttk.Entry(addr_frame, textvariable=self.addr_var, width=10)
        addr_entry.grid(row=0, column=1, padx=5, pady=5, sticky="w")
        
        # Image selection
        img_frame = ttk.LabelFrame(main_frame, text="Image Selection", padding="5")
        img_frame.pack(fill="x", pady=5)
        
        ttk.Button(img_frame, text="Select Image", command=self.select_image).grid(row=0, column=0, padx=5, pady=5)
        self.image_label = ttk.Label(img_frame, text="No image selected")
        self.image_label.grid(row=0, column=1, padx=5, pady=5, sticky="w")
        
        # Image preview
        preview_frame = ttk.LabelFrame(main_frame, text="Image Preview", padding="5")
        preview_frame.pack(fill="both", expand=True, pady=5)
        
        self.preview_canvas = tk.Canvas(preview_frame, bg="lightgray", width=466, height=466)
        self.preview_canvas.pack(padx=5, pady=5)
        
        # Flash button
        self.flash_button = ttk.Button(main_frame, text="Convert and Flash Image", 
                                      command=self.start_flash_thread, state="disabled")
        self.flash_button.pack(pady=10)
        
        # Progress bar
        self.progress_var = tk.DoubleVar()
        self.progress = ttk.Progressbar(main_frame, variable=self.progress_var, maximum=100)
        self.progress.pack(fill="x", pady=5)
        
        # Status
        status_frame = ttk.Frame(main_frame)
        status_frame.pack(fill="x", pady=5)
        
        ttk.Label(status_frame, text="Status:").pack(side="left", padx=5)
        self.status_var = tk.StringVar(value="Ready")
        status_label = ttk.Label(status_frame, textvariable=self.status_var)
        status_label.pack(side="left", padx=5, fill="x", expand=True)
    
    def refresh_ports(self):
        ports = [p.device for p in serial.tools.list_ports.comports()]
        if ports:
            self.port_combo['values'] = ports
            self.port_var.set(ports[0])
        else:
            self.port_combo['values'] = ["No ports available"]
            self.port_var.set("")
    
    def select_image(self):
        file_path = filedialog.askopenfilename(
            title="Select Image",
            filetypes=[("Image files", "*.png *.jpg *.jpeg *.bmp")]
        )
        
        if not file_path:
            return
            
        self.image_path = file_path
        self.image_label.config(text=os.path.basename(file_path))
        
        # Process and preview image
        try:
            self.process_image_preview()
            self.flash_button.config(state="normal")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to process image: {str(e)}")
            self.flash_button.config(state="disabled")
    
    def process_image_preview(self):
        # Open and resize image for preview
        img = Image.open(self.image_path)
        img = img.resize((466, 466), Image.LANCZOS)
        
        # Display preview
        self.preview_img = ImageTk.PhotoImage(img)
        self.preview_canvas.create_image(233, 233, image=self.preview_img)
    
    def start_flash_thread(self):
        # Disable UI elements during flashing
        self.flash_button.config(state="disabled")
        
        # Start flashing in a separate thread
        thread = threading.Thread(target=self.convert_and_flash_image)
        thread.daemon = True
        thread.start()
    
    def convert_and_flash_image(self):
        try:
            self.status_var.set("Converting image...")
            self.progress_var.set(10)
            self.root.update_idletasks()
            
            # Open and resize image
            img = Image.open(self.image_path)
            img = img.resize((466, 466), Image.LANCZOS)
            
            # Convert to RGB if needed
            if img.mode != "RGB":
                img = img.convert("RGB")
            
            width, height = img.size
            self.status_var.set(f"Converting image: {width}x{height}")
            self.progress_var.set(30)
            self.root.update_idletasks()
            
            # Create binary data
            header = struct.pack("<HHB", width, height, 4)  # 4 = LV_IMG_CF_TRUE_COLOR
            padding = b'\x00' * 3  # Padding to make header 8 bytes
            
            # Convert pixels to RGB565
            pixels = []
            for y in range(height):
                for x in range(width):
                    r, g, b = img.getpixel((x, y))
                    rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
                    pixels.append((rgb565 >> 8) & 0xFF)
                    pixels.append(rgb565 & 0xFF)
                
                # Update progress for large images
                if y % 20 == 0:
                    progress = 30 + (y / height * 30)
                    self.progress_var.set(progress)
                    self.root.update_idletasks()
            
            # Create binary file
            bin_file = "image.bin"
            with open(bin_file, "wb") as f:
                f.write(header + padding + bytes(pixels))
            
            file_size = os.path.getsize(bin_file)
            self.status_var.set(f"Created binary file: {bin_file}, size: {file_size} bytes")
            self.progress_var.set(70)
            self.root.update_idletasks()
            
            # Flash the image
            port = self.port_var.get()
            address = self.addr_var.get()
            
            if not port:
                raise ValueError("No serial port selected")
            
            cmd = [
                "python", "-m", "esptool",
                "--chip", "esp32s3",
                "--port", port,
                "--baud", "460800",
                "write_flash",
                address, bin_file
            ]
            
            self.status_var.set(f"Flashing to address {address}...")
            self.progress_var.set(80)
            self.root.update_idletasks()
            
            # Run the flashing command
            process = subprocess.Popen(
                cmd, 
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True
            )
            
            # Wait for the process to complete
            stdout, stderr = process.communicate()
            
            if process.returncode == 0:
                self.status_var.set("Image flashed successfully!")
                self.progress_var.set(100)
                messagebox.showinfo("Success", "Image flashed successfully!")
            else:
                error_msg = stderr or stdout
                self.status_var.set(f"Error: {error_msg}")
                messagebox.showerror("Error", f"Failed to flash image:\n{error_msg}")
                
        except Exception as e:
            self.status_var.set(f"Error: {str(e)}")
            messagebox.showerror("Error", str(e))
        finally:
            # Re-enable UI elements
            self.flash_button.config(state="normal")

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageFlasherApp(root)
    root.mainloop()
