# Lab Streaming Layer driver for MCP3008
This is a driver for collecting data from the MCP3008 ADC chip and streaming it over LSL using a
Raspberry pi running Rasbian as the host device. The sampled channels, stream name, uid, etc. Can be
adjusted at run through multiple commandline flags.

## Getting Setup
The following documents how to get a minimal setup with the MCP3008 on a breadboard.

### Wiring
The MCP3008 should be hooked up to the pi's SPI bus and an analog signal to sample. For our setup,
we followed Adafruit's well documented tutorial found
[here](https://learn.adafruit.com/mcp3008-spi-adc/python-circuitpython) for wiring up the MCP3008 to
the Raspberry Pi.

### Configuration
Starting with a fresh raspbian install on the pi we need to enable the I2C and SPI interfaces.
Again we found Adafruit tutorials useful for this step:

* [I2C](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-spi)
* [SPI](https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-spi)

### Prerequisites
The only required library (excluding the default linux, I2C, SPI libraries) is
[libLSL](https://github.com/sccn/liblsl). Since libLSL doesn't target Rasbian in it's releases, it's
necessary to build a `.deb` package for raspbian. While the README in libLSL recommends using
`standalone_compilation_linux.sh` for computers like the raspberry pi, this simply creates the
library binary in the project folder. Below is documentation for building and installing libLSL.

#### Build & Install libLSL
Clone or download the libLSL project:

```bash
git clone https://github.com/sccn/liblsl.git
```
or grab the latest source from the releases tab: <https://github.com/sccn/liblsl/releases>

`cd` into the project folder and make the build folder and descend into the folder and generate the
build target using `cmake`:

```bash
cd liblsl
mkdir build && cd build
cmake ..
```

Finally build and compile the `.deb` package and install it.
```bash
make -j package
sudo dpkg -i *.deb
```

### Building
Assuming the previous steps have gone smoothly (and I haven't missed a dependency), the project
should able to be built by running the `make` in the root of the project.

Clone the repo
```bash
git clone https://github.com/neurotechuoft/mcp3008lsl
```

Descending into the project and build it.

```bash
cd
make
```

## Usage

TODO
