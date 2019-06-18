# Bios Boot Tutorial

The `bios-boot-tutorial.py` script simulates the DAio bios boot sequence.

``Prerequisites``:

1. Python 3.x
2. CMake
3. git

``Steps``:

1. Install DAio binaries by following the steps outlined in below tutorial
[Install DAio binaries](https://github.com/DAio/DA#mac-os-x-brew-install)

2. Install DAio.cdt binaries by following the steps outlined in below tutorial
[Install DAio.cdt binaries](https://github.com/DAio/DAio.cdt#binary-releases)

3. Compile DAio.contracts

```bash
$ cd ~
$ git clone https://github.com/DAio/DAio.contracts.git
$ cd ./DAio.contracts/
$ ./build.sh
$ pwd

```

4. Make note of the directory where the contracts were compiled
The last command in the previous step printed on the bash console the contracts' directory, make note of it, we'll reference it from now on as `DAio_CONTRACTS_DIRECTORY`

5. Launch the `bios-boot-tutorial.py` script
Minimal command line to launch the script below, make sure you replace `DAio_CONTRACTS_DIRECTORY` with actual directory

```bash
$ cd ~
$ git clone https://github.com/DAio/DA.git
$ cd ./DA/tutorials/bios-boot-tutorial/
$ python3 bios-boot-tutorial.py --cleos="cleos --wallet-url http://127.0.0.1:6666 " --nodeos=nodeos --keosd=keosd --contracts-dir="/DAio_CONTRACTS_DIRECTORY/" -a

```

See [DAio Documentation Wiki: Tutorial - Bios Boot](https://github.com/DAio/DA/wiki/Tutorial-Bios-Boot-Sequence) for additional information.