import time
import serial
import matplotlib.pyplot as plt

def read_value(ser):
    while True:
        try:
            line = ser.readline().decode('ascii').strip()
            v, t = map(float, line.split())
            return v, t
        except (ValueError, UnicodeDecodeError):
            continue

def main():
    ser = serial.Serial(port='COM5', baudrate=115200, timeout=0.0)

    if ser.is_open:
        print(f"Port {ser.name} opened")
    else:
        print(f"Port {ser.name} closed")
        return

    # Запускаем телеметрию
    ser.write("tm_start\n".encode('ascii'))
    print("Telemetry started")

    measure_voltage_V = []
    measure_temperature_C = []
    measure_ts = []

    start_ts = time.time()

    try:
        while True:
            ts = time.time() - start_ts
            voltage_V, temp_C = read_value(ser)

            measure_ts.append(ts)
            measure_voltage_V.append(voltage_V)
            measure_temperature_C.append(temp_C)

            print(f'{voltage_V:.3f} V - {temp_C:.1f}°C - {ts:.2f}s')

            # Небольшая задержка для читаемости (но данные уже идут с периодом 100 мс)
            time.sleep(0.01)

    except KeyboardInterrupt:
        print("\nInterrupted by user")
    finally:
        # Останавливаем телеметрию
        ser.write("tm_stop\n".encode('ascii'))
        print("Telemetry stopped")
        ser.close()
        print("Port closed")

        # Графики
        plt.figure(figsize=(8, 6))
        plt.subplot(2, 1, 1)
        plt.plot(measure_ts, measure_voltage_V, 'b-')
        plt.title('Напряжение на GPIO26 (телеметрия)')
        plt.xlabel('Время, с')
        plt.ylabel('Напряжение, В')
        plt.grid(True)

        plt.subplot(2, 1, 2)
        plt.plot(measure_ts, measure_temperature_C, 'r-')
        plt.title('Температура кристалла RP2040 (телеметрия)')
        plt.xlabel('Время, с')
        plt.ylabel('Температура, °C')
        plt.grid(True)

        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    main()