# AESFinder

Dll that allows you to dump the main AES key of Fortnite through game's memory (original project of <a href="https://github.com/Fischsalat">Fischsalat</a>, reworked by me).

> [!WARNING]
> Do not inject this DLL with the AntiCheat enabled. Use a private server like <a href="https://discord.gg/HR2TjuEb">Carbon</a>.

## Installation

1. Clone the source code with <a href="https://git-scm.com/downloads">git</a>: `git clone https://github.com/djlorenzouasset/AESFinder`

2. Build the project using <a href="https://visualstudio.microsoft.com/downloads">Visual Studio 2022</a> or <a href="https://www.jetbrains.com/rider">JetBrains Rider</a>.

## How to use

1. Run Carbon private server (get it from <a href="https://discord.gg/HR2TjuEb">here</a>).

2. Use <a href="https://mega.nz/file/MUhUlaqa#PI0ubSy_sQc4GouDPnZbulNSgQRUNIJeXNwLb2LnKhk">UUUClient</a> (or others injectors) for inject the DLL in the Fortnite process.

3. Once the DLL gets injected, a file will be saved in the default directory `C:/AESFinder` containing the dumped AES key.

> [!IMPORTANT]
> Inject the DLL into Fortnite right after starting it.

## Program Settings

This program also allow you to decide if save the dumped AES key in a file or not, and in what directory save it (default is `C:/AESFinder`). You can change these settings in the `globals.h` file:

- `Settings::bShouldSaveFile`: Leave it as default if you want to save the dumped AES key or set it to `false` for skip the file save.
- `Settings::SavePath`: Leave it as default if you want to use the default directory `C:/AESFinder`, or set a new one (the program will automatically create a new one if the inserted one doesn't exist).

## Contributing 
Contributions are welcome, feel free to create pull requests or report issues on the GitHub repository.

## License
This project is licensed under the MIT License.
