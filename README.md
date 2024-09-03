# Stellar Combat

Stellar Combat is an exciting game inspired by the classic River Raid. In this game, you steer a spaceship and fight against enemies. Developed using the C language and SDL2 library, it features beautiful graphics and amazing sound effects. The game is cross-platform, thanks to its Makefile, and includes a logging system to help developers detect errors quickly.

## Features

- **Beautiful Graphics**: Stunning visuals generated using AI.
- **Amazing Sound Effects**: Immersive audio experience.
- **Cross-Platform**: Easily build and run on different platforms using the provided Makefile.
- **Logging System**: Efficient error detection for developers.
- **Modular Code**: Separate interfaces for graphics and sound effects.

## Screenshots

| ![Game Logo](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/logo.png) | ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/1.png) |
|----------------------------------------------|----------------------------------------------|
| ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/2.png) | ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/3.png) |
| ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/4.png) | ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/5.png) |
| ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/6.png) | ![Game Screenshot](https://github.com/AminRezaeeyan/Stellar-Combat/blob/main/screenshots/7.png) |

## Installation

To run Stellar Combat, you need to have `make` and the following SDL2 libraries installed: `sdl2`, `sdl_image`, `sdl_ttf`, `sdl_mixer`.

1. Clone the repository:

   ```sh
   git clone https://github.com/AminRezaeeyan/Stellar-Combat.git
   cd Stellar-Combat
   ```

2. Build the game:

   ```sh
   make
   ```

3. Run the game:
   ```sh
   build/StellarCombat
   ```

## Contribution

Contributions are welcome! If you want to contribute, please fork the repository and create a pull request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Developer Notes

For developers who want to use this code, there are two files named `graphics.c` and `sounds.c` that serve as interfaces between the SDL library and the main application. Instead of directly using SDL methods inside the main application, these interfaces provide a modular approach for handling graphics and sound effects. This method is akin to creating small libraries for our app.

---

Enjoy playing Stellar Combat!
