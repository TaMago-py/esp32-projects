<div align="center"> 
  
  <h1> screen_test </h1>

</div>

<br>
<p align="center">
  <img src="https://github.com/user-attachments/assets/e644b557-1192-4abd-b840-029e6ed06d8a" width="25%" alt="TaMago-roll"><img src="https://github.com/user-attachments/assets/e644b557-1192-4abd-b840-029e6ed06d8a" width="25%" alt="TaMago-roll"><img src="https://github.com/user-attachments/assets/e644b557-1192-4abd-b840-029e6ed06d8a" width="25%" alt="TaMago-roll"><img src="https://github.com/user-attachments/assets/e644b557-1192-4abd-b840-029e6ed06d8a" width="25%" alt="TaMago-roll">
</p>

### What does it do?

Displays a __rotating 3D cube__ on the screen. You can __interact__ with the animation via buttons and potentiometer, which are as follow:

+ Toggle __inverted__ colors via button on pin 18.
+ Toggle __solid__ cube mode via button on pin 19.
+ Adjust the __distance__ from the cube via potentiometer on pin 35.

### Tools

+ ![Adafruit](https://img.shields.io/badge/Adafruit-%23000000.svg?style=for-the-badge&logo=adafruit&logoColor=white)
+ ![Arduino](https://img.shields.io/badge/Arduino-%2300979D.svg?style=for-the-badge&logo=Arduino&logoColor=white)
+ ![C++](https://img.shields.io/badge/c%2B%2B-%2300599C.svg?style=for-the-badge&logo=cplusplus&logoColor=white)
+ ![Espressif](https://img.shields.io/badge/espressif-%23E7352C.svg?style=for-the-badge&logo=espressif&logoColor=white)
+ ![PlatformIO](https://img.shields.io/badge/platformio-%23000.svg?style=for-the-badge&logo=platformio&logoColor=F5822A)

### specifications

Built with:

+ Adafruit's ___SSD1306___ oled screen.
+ Espressif's ___ESP-WROOM-32___.
+ ___PlatformIO___'s framework.
+ ___Arduino___'s library.

### Procedure

> __Divided into sections__. Every section is an important individual part of the code.

<details>
  <summary> Cube's vertices and 3D space orientation </summary>
  <br>

  The first step was to represent the cube's vertices:

  ``` c++

const float vertices[8][3] = {
  {-20, -20, -20}, {20, -20, -20}, {20, 20, -20}, {-20, 20, -20}, // Front face
  {-20, -20, 20}, {20, -20, 20}, {20, 20, 20}, {-20, 20, 20} // Back face
};

```

First I draw the 3D space as it is supposed to be in a screen, with the $y$ axis pointing down. In this model the 3D space looks like this:

<img width="80%" alt="3D-space" src="https://github.com/user-attachments/assets/e9e14c44-cc3f-4b13-a335-71352c8ee709"/>

</details>

