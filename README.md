# Ejemplo de uso de SPI en modo Master

Este proyecto muestra cómo configurar el **ESP32** en modo **SPI Master** para comunicarse con dos dispositivos esclavos ficticios. Se utilizan señales simuladas para representar la interacción entre el master y los esclavos, lo que permite visualizar la comunicación mediante un analizador lógico.

---

## 📌 Objetivos

- Configurar el ESP32 como dispositivo maestro SPI.
- Simular dos esclavos SPI.
- Visualizar la comunicación SPI con un analizador lógico.
- Comprender el uso de pines `MOSI`, `MISO`, `SCLK` y `CS`.

---

## 🧰 Requisitos

- ESP32 (cualquier modelo compatible)
- Analizador lógico (opcional pero recomendado)

---



## ⚙️ Configuración de pines

| Función | Pin ESP32 (ejemplo) |
|--------|----------------------|
| MOSI   | GPIO 25              |
| MISO   | GPIO 23              |
| SCLK   | GPIO 19              |
| CS1    | GPIO 22              |
| CS2    | GPIO 21              |


> ⚠️ Asegurate de modificar los pines si estás usando otro modelo de ESP32.

---

## 📸 Capturas del Analizador Lógico

A continuación, se muestran ejemplos de señales SPI capturadas con un analizador lógico. Estas capturas permiten observar la secuencia de bits y la activación de los pines `CS`.



![Transferencias SPI a ambos esclavos](imagenes/spi_slave1.png)



### 🖼️ Transferencia con el esclavo 2

Configuracion del dispositivo 2:
```c
    spi_device_interface_config_t dev2_config ={
        .clock_speed_hz = 10000,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT, //
        .spics_io_num = SPI_CS_DEV2,
        .queue_size = 10,
        .pre_cb = NULL,
        .post_cb=NULL,
        .flags = SPI_DEVICE_NO_DUMMY
    };
```


![Transferencia SPI con esclavo 2](imagenes/spi_slave2.png)

---

## 💻 Código de ejemplo

A continuación se muestra un fragmento de código para inicializar el SPI en modo master en el ESP32:

```cpp
#include <SPI.h>

#define CS1 5
#define CS2 17

void setup() {
  Serial.begin(115200);
  
  pinMode(CS1, OUTPUT);
  pinMode(CS2, OUTPUT);
  
  digitalWrite(CS1, HIGH);
  digitalWrite(CS2, HIGH);

  SPI.begin(); // MOSI=23, MISO=19, SCLK=18 por defecto

  // Transferencia al esclavo 1
  digitalWrite(CS1, LOW);
  SPI.transfer(0x42);
  digitalWrite(CS1, HIGH);

  delay(100);

  // Transferencia al esclavo 2
  digitalWrite(CS2, LOW);
  SPI.transfer(0x55);
  digitalWrite(CS2, HIGH);
}

void loop() {
  // Nada en loop
}
``
