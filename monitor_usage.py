import serial
import time
import psutil
import GPUtil
import argparse

def get_cpu_usage():
    return int(psutil.cpu_percent(interval=1))

def get_gpu_usage():
    try:
        gpus = GPUtil.getGPUs()
        if gpus:
            return int(gpus[0].load * 100)
        return 0
    except:
        return 0

def main():
    parser = argparse.ArgumentParser(description='Monitor and send CPU/GPU usage over serial')
    parser.add_argument('port', help='Serial port (e.g., COM3 or /dev/ttyUSB0)')
    parser.add_argument('--baud', type=int, default=115200, help='Baud rate (default: 115200)')
    args = parser.parse_args()

    try:
        # Open serial port with flow control disabled
        ser = serial.Serial(
            args.port, 
            args.baud, 
            timeout=20,
            rtscts=False,
            dsrdtr=False
        )
        
        # Explicitly disable DTR and RTS
        ser.dtr = False
        ser.rts = False
        
        print(f"Connected to {args.port} at {args.baud} baud")
        
        # Give the ESP32 time to stabilize
        time.sleep(2)
        
        reconnect_count = 0
        max_reconnects = 3
        
        while True:
            try:
                # Get usage data
                cpu_usage = get_cpu_usage()
                gpu_usage = get_gpu_usage()
                
                # Format the data
                data = f"CPU:{cpu_usage} GPU:{gpu_usage}\n"
                
                # Send over serial
                ser.write(data.encode())
                print(f"Sent: {data.strip()}")
                
                # Wait a bit before next update
                time.sleep(1)
                
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                reconnect_count += 1
                
                if reconnect_count > max_reconnects:
                    print("Too many reconnection attempts, exiting")
                    break
                    
                print(f"Attempting to reconnect ({reconnect_count}/{max_reconnects})...")
                try:
                    ser.close()
                    time.sleep(2)  # Give device time to reset
                    ser = serial.Serial(
                        args.port, 
                        args.baud, 
                        timeout=20,
                        rtscts=False,
                        dsrdtr=False
                    )
                    ser.dtr = False
                    ser.rts = False
                    time.sleep(2)  # Stabilization time
                    print("Reconnected successfully")
                except Exception as e:
                    print(f"Failed to reconnect: {e}")
            
    except KeyboardInterrupt:
        print("\nMonitoring stopped by user")
    finally:
        if 'ser' in locals() and ser.is_open:
            # Carefully close the port without toggling DTR/RTS
            ser.dtr = False
            ser.rts = False
            time.sleep(0.1)
            ser.close()
            print("Serial port closed")

if __name__ == "__main__":
    main()
