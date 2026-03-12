import time
import serial
import matplotlib.pyplot as plt

def read_value(ser):
    while True:
        try:
            line = ser.readline().decode('ascii').strip()
            value = float(line)
            return value
        except (ValueError, UnicodeDecodeError):
            continue

def main():
    # Настройте порт под вашу систему (например, 'COM3' или '/dev/ttyACM0')
    ser = serial.Serial(port='COM5', baudrate=115200, timeout=0.0)

    if ser.is_open:
        print(f"Port {ser.name} opened")
    else:
        print(f"Port {ser.name} closed")
        return

    measure_voltage_V = []
    measure_temperature_C = []
    measure_ts = []

    start_ts = time.time()

    try:
        while True:
            ts = time.time() - start_ts

            ser.write("get_adc\n".encode('ascii'))
            voltage_V = read_value(ser)

            ser.write("get_temp\n".encode('ascii'))
            temp_C = read_value(ser)

            measure_ts.append(ts)
            measure_voltage_V.append(voltage_V)
            measure_temperature_C.append(temp_C)

            print(f'{voltage_V:.3f} V - {temp_C:.1f}°C - {ts:.2f}s')

            time.sleep(0.1)

    except KeyboardInterrupt:
        print("\nInterrupted by user")
    finally:
        ser.close()
        print("Port closed")

        # Построение графиков
        plt.figure(figsize=(8, 6))

        plt.subplot(2, 1, 1)
        plt.plot(measure_ts, measure_voltage_V, 'b-')
        plt.title('Напряжение на GPIO26')
        plt.xlabel('Время, с')
        plt.ylabel('Напряжение, В')
        plt.grid(True)

        plt.subplot(2, 1, 2)
        plt.plot(measure_ts, measure_temperature_C, 'r-')
        plt.title('Температура кристалла RP2040')
        plt.xlabel('Время, с')
        plt.ylabel('Температура, °C')
        plt.grid(True)

        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    main()