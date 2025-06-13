import tkinter as tk
from tkinter import messagebox, scrolledtext
import subprocess
import struct
import os
import sys
import platform
import threading
# HID key code mapping (partial)
KEY_MAPPING = {
    'a': 0x04, 'b': 0x05, 'c': 0x06, 'd': 0x07, 'e': 0x08, 'f': 0x09,
    'g': 0x0A, 'h': 0x0B, 'i': 0x0C, 'j': 0x0D, 'k': 0x0E, 'l': 0x0F,
    'm': 0x10, 'n': 0x11, 'o': 0x12, 'p': 0x13, 'q': 0x14, 'r': 0x15,
    's': 0x16, 't': 0x17, 'u': 0x18, 'v': 0x19, 'w': 0x1A, 'x': 0x1B,
    'y': 0x1C, 'z': 0x1D, '1': 0x1E, '2': 0x1F, '3': 0x20, '4': 0x21,
    '5': 0x22, '6': 0x23, '7': 0x24, '8': 0x25, '9': 0x26, '0': 0x27,
    'enter': 0x28, 'esc': 0x29, 'backspace': 0x2A, 'tab': 0x2B, 'space': 0x2C,
    'left': 0x50, 'right': 0x4F, 'up': 0x52, 'down': 0x51,
    'f1': 0x3A, 'f2': 0x3B, 'f3': 0x3C, 'f4': 0x3D,
    'f5': 0x3E, 'f6': 0x3F, 'f7': 0x40, 'f8': 0x41,
    'f9': 0x42, 'f10': 0x43, 'f11': 0x44, 'f12': 0x45
}

# Modifier keys
MODIFIER_MAPPING = {
    'ctrl': 0x01,
    'shift': 0x02,
    'alt': 0x04,
    'win': 0x08
}

class ConfigApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Knob Configuration Tool")
        self.root.geometry("700x800")  # Increased size to accommodate profile 2
        
        # Variables for key selections
        self.left_key = tk.StringVar(value="none")
        self.left_mod = tk.StringVar(value="none")
        self.right_key = tk.StringVar(value="none")
        self.right_mod = tk.StringVar(value="none")
        self.left_key2 = tk.StringVar(value="none")  # Profile 2
        self.left_mod2 = tk.StringVar(value="none")  # Profile 2
        self.right_key2 = tk.StringVar(value="none")  # Profile 2
        self.right_mod2 = tk.StringVar(value="none")  # Profile 2
        
        self.create_widgets()
    

    def safe_log_message(self, message):
        """Thread-safe method to update the log text widget"""
        self.root.after_idle(lambda: self.log_message(message))
        
    def create_widgets(self):
        # Title
        tk.Label(self.root, text="Knob Configuration Tool", font=("Arial", 16)).pack(pady=10)
        
        # Profile 1 frame
        profile1_frame = tk.LabelFrame(self.root, text="Profile 1", font=("Arial", 12))
        profile1_frame.pack(fill="x", padx=20, pady=10)
        
        # Left knob frame
        left_frame = tk.Frame(profile1_frame)
        left_frame.pack(fill="x", padx=20, pady=10)
        
        tk.Label(left_frame, text="Left Rotation:", font=("Arial", 12)).grid(row=0, column=0, sticky="w")
        
        # Left key dropdown
        tk.Label(left_frame, text="Key:").grid(row=1, column=0, sticky="w")
        left_key_menu = tk.OptionMenu(left_frame, self.left_key, "none", *sorted(KEY_MAPPING.keys()))
        left_key_menu.grid(row=1, column=1, sticky="w")
        
        # Left modifier dropdown
        tk.Label(left_frame, text="Modifier:").grid(row=2, column=0, sticky="w")
        left_mod_menu = tk.OptionMenu(left_frame, self.left_mod, "none", *sorted(MODIFIER_MAPPING.keys()))
        left_mod_menu.grid(row=2, column=1, sticky="w")
        
        # Right knob frame
        right_frame = tk.Frame(profile1_frame)
        right_frame.pack(fill="x", padx=20, pady=10)
        
        tk.Label(right_frame, text="Right Rotation:", font=("Arial", 12)).grid(row=0, column=0, sticky="w")
        
        # Right key dropdown
        tk.Label(right_frame, text="Key:").grid(row=1, column=0, sticky="w")
        right_key_menu = tk.OptionMenu(right_frame, self.right_key, "none", *sorted(KEY_MAPPING.keys()))
        right_key_menu.grid(row=1, column=1, sticky="w")
        
        # Right modifier dropdown
        tk.Label(right_frame, text="Modifier:").grid(row=2, column=0, sticky="w")
        right_mod_menu = tk.OptionMenu(right_frame, self.right_mod, "none", *sorted(MODIFIER_MAPPING.keys()))
        right_mod_menu.grid(row=2, column=1, sticky="w")

        # Profile 2 frame
        profile2_frame = tk.LabelFrame(self.root, text="Profile 2", font=("Arial", 12))
        profile2_frame.pack(fill="x", padx=20, pady=10)
        
        # Left knob frame for profile 2
        left_frame2 = tk.Frame(profile2_frame)
        left_frame2.pack(fill="x", padx=20, pady=10)
        
        tk.Label(left_frame2, text="Left Rotation:", font=("Arial", 12)).grid(row=0, column=0, sticky="w")
        
        # Left key dropdown for profile 2
        tk.Label(left_frame2, text="Key:").grid(row=1, column=0, sticky="w")
        left_key_menu2 = tk.OptionMenu(left_frame2, self.left_key2, "none", *sorted(KEY_MAPPING.keys()))
        left_key_menu2.grid(row=1, column=1, sticky="w")
        
        # Left modifier dropdown for profile 2
        tk.Label(left_frame2, text="Modifier:").grid(row=2, column=0, sticky="w")
        left_mod_menu2 = tk.OptionMenu(left_frame2, self.left_mod2, "none", *sorted(MODIFIER_MAPPING.keys()))
        left_mod_menu2.grid(row=2, column=1, sticky="w")
        
        # Right knob frame for profile 2
        right_frame2 = tk.Frame(profile2_frame)
        right_frame2.pack(fill="x", padx=20, pady=10)
        
        tk.Label(right_frame2, text="Right Rotation:", font=("Arial", 12)).grid(row=0, column=0, sticky="w")
        
        # Right key dropdown for profile 2
        tk.Label(right_frame2, text="Key:").grid(row=1, column=0, sticky="w")
        right_key_menu2 = tk.OptionMenu(right_frame2, self.right_key2, "none", *sorted(KEY_MAPPING.keys()))
        right_key_menu2.grid(row=1, column=1, sticky="w")
        
        # Right modifier dropdown for profile 2
        tk.Label(right_frame2, text="Modifier:").grid(row=2, column=0, sticky="w")
        right_mod_menu2 = tk.OptionMenu(right_frame2, self.right_mod2, "none", *sorted(MODIFIER_MAPPING.keys()))
        right_mod_menu2.grid(row=2, column=1, sticky="w")
        
        # Port selection
        port_frame = tk.Frame(self.root)
        port_frame.pack(fill="x", padx=20, pady=10)
        
        tk.Label(port_frame, text="COM Port:").pack(side="left")
        self.port_entry = tk.Entry(port_frame, width=10)
        self.port_entry.pack(side="left", padx=5)
        self.port_entry.insert(0, "COM3")  # Default port
        
        # Flash button
        tk.Button(self.root, text="Save and Flash Configuration", 
                  command=self.flash_config, bg="#4CAF50", fg="white", 
                  font=("Arial", 12)).pack(pady=10)
        
        # Add log window
        log_frame = tk.Frame(self.root)
        log_frame.pack(fill="both", expand=True, padx=20, pady=10)
        
        tk.Label(log_frame, text="Command Output:", font=("Arial", 10)).pack(anchor="w")
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=10)
        self.log_text.pack(fill="both", expand=True)
        
        # Add a clear log button
        tk.Button(log_frame, text="Clear Log", command=self.clear_log).pack(anchor="e", pady=5)
    
    def clear_log(self):
        self.log_text.delete(1.0, tk.END)
    
    def log_message(self, message):
        self.log_text.insert(tk.END, message + "\n")
        self.log_text.see(tk.END)  # Auto-scroll to the end
    
    def flash_config(self):
        try:
            self.log_message("Starting configuration process...")
            
            # Get selected values
            left_key_val = KEY_MAPPING.get(self.left_key.get(), 0)
            left_mod_val = MODIFIER_MAPPING.get(self.left_mod.get(), 0)
            right_key_val = KEY_MAPPING.get(self.right_key.get(), 0)
            right_mod_val = MODIFIER_MAPPING.get(self.right_mod.get(), 0)
            left_key2_val = KEY_MAPPING.get(self.left_key2.get(), 0)  # Profile 2
            left_mod2_val = MODIFIER_MAPPING.get(self.left_mod2.get(), 0)  # Profile 2
            right_key2_val = KEY_MAPPING.get(self.right_key2.get(), 0)  # Profile 2
            right_mod2_val = MODIFIER_MAPPING.get(self.right_mod2.get(), 0)  # Profile 2
            
            # Create binary configuration
            config_data = struct.pack('<BBBBBBBBI', 
                                    left_key_val, 
                                    left_mod_val,
                                    right_key_val,
                                    right_mod_val,
                                    left_key2_val,  # Profile 2
                                    left_mod2_val,  # Profile 2
                                    right_key2_val,  # Profile 2
                                    right_mod2_val,  # Profile 2
                                    0xABCD1234)  # Magic number
            
            # Save to file
            with open("user_config.bin", "wb") as f:
                f.write(config_data)
            
            # Start flashing in a separate thread
            threading.Thread(target=self._run_flash_process, 
                           args=(left_key_val, left_mod_val, right_key_val, right_mod_val,
                                left_key2_val, left_mod2_val, right_key2_val, right_mod2_val)).start()
            
        except Exception as e:
            error_message = f"An error occurred: {str(e)}"
            self.log_message(error_message)
            messagebox.showerror("Error", error_message)

    def _run_flash_process(self, left_key_val, left_mod_val, right_key_val, right_mod_val,
                         left_key2_val, left_mod2_val, right_key2_val, right_mod2_val):
        try:
            self.log_message(f"Selected values:")
            self.log_message(f"Profile 1:")
            self.log_message(f"Left key: {self.left_key.get()} (0x{left_key_val:02X})")
            self.log_message(f"Left modifier: {self.left_mod.get()} (0x{left_mod_val:02X})")
            self.log_message(f"Right key: {self.right_key.get()} (0x{right_key_val:02X})")
            self.log_message(f"Right modifier: {self.right_mod.get()} (0x{right_mod_val:02X})")
            self.log_message(f"Profile 2:")
            self.log_message(f"Left key: {self.left_key2.get()} (0x{left_key2_val:02X})")
            self.log_message(f"Left modifier: {self.left_mod2.get()} (0x{left_mod2_val:02X})")
            self.log_message(f"Right key: {self.right_key2.get()} (0x{right_key2_val:02X})")
            self.log_message(f"Right modifier: {self.right_mod2.get()} (0x{right_mod2_val:02X})")
            
            port = self.port_entry.get()
            
            # Use system Python and esptool module
            cmd = [
                "python", "-m", "esptool",  # Use Python module format
                "-p", port,
                "-b", "460800",
                "--before", "default_reset",
                "--after", "hard_reset",
                "--chip", "esp32s3",
                "write_flash",
                "--flash_mode", "dio",
                "--flash_freq", "80m",
                "--flash_size", "8MB",
                "0x410000", "user_config.bin"
            ]
            
            self.log_message(f"Executing command: {' '.join(cmd)}")
            # Run the command with real-time output
            process = subprocess.Popen(
                cmd, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
                universal_newlines=True
            )
            
            print("im here")

            # Process output in real-time
            for line in iter(process.stdout.readline, ''):
                if line:
                    # Use after_idle to update GUI from a different thread
                    self.root.after_idle(lambda msg=line: self.log_message(msg.strip()))
            
            # Get return code
            return_code = process.wait()
            
            # Get any remaining output
            stdout, stderr = process.communicate()
            
            if stdout:
                self.root.after_idle(lambda msg=stdout: self.log_message(msg))
            if stderr:
                self.root.after_idle(lambda msg=f"ERROR: {stderr}": self.log_message(msg))
            
            if return_code == 0:
                self.root.after_idle(lambda: self.log_message("Flash completed successfully!"))
                self.root.after_idle(lambda: messagebox.showinfo("Success", "Configuration flashed successfully!"))
            else:
                self.root.after_idle(lambda: self.log_message(f"Flash failed with return code: {return_code}"))
                self.root.after_idle(lambda: messagebox.showerror("Error", "Failed to flash configuration. See log for details."))
                
        except Exception as e:
            error_message = f"An error occurred: {str(e)}"
            self.root.after_idle(lambda msg=error_message: self.log_message(msg))
            self.root.after_idle(lambda msg=error_message: messagebox.showerror("Error", msg))






if __name__ == "__main__":
    root = tk.Tk()
    app = ConfigApp(root)
    root.mainloop()
