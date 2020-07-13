# Lab Streaming Layer driver for MCP3008 
![C CI](https://github.com/neurotechuoft/Hardware/workflows/C%20CI/badge.svg)

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

The driver can be started as follows:
```bash
usage: ./mcp3008lsl [-f freq in hz] [-n stream name] [-u uid] [-t type]  [0-7] [[0-7]...]
```

`-f` specifies the sampling frequency of the given channels in hertz. In theory the chip can
sample at a rate of 10000hz, but in rudimentary testing I've found that this is rarely achieved
likely due to a number of factors contributing to overhead. Default sampling is 500hz.

`-n` specifies the name of the stream over the lsl layer. Defaults to "SendDataC".

`-u` the unique identifier of the device. NOTE: make sure this is a unique value if you plan to
have multiple streaming devices. Defaults to "325wqer4354".

`-t` specifies the type of the data being streamed from the MCP3008 for the LSL layer. Defaults
to "EKG".

`[0-7]` specifies the input pin assigned to the first channel value. Subsequent specified
channels will map to the next channel for a total of `n` possible channels.

### Examples
Start a stream of 2 channels from inputs 5 and 7:
```bash
./mcp3008lsl 5 7
```

Start a stream of 8 channels from the inputs in reverse order at 1000hz:
```bash
./mcp3008lsl -f 1000 7 6 5 4 3 2 1 0
```
