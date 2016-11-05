Platfrom:

    Raspberry Pi 0 -  https://www.raspberrypi.org/products/pi-zero/

    OS: Raspbian Jessie --https://www.raspberrypi.org/downloads/raspbian/

    IR Sensor: Panasonic Grid Eye -- https://na.industrial.panasonic.com/products/sensors/sensors-automotive-industrial-applications/grid-eye-infrared-array-sensor

Installation Process:

Part A:
install libftdi version 1.1.2

First: Install Dependencies as follow:
1. Install cmake
	
    sudo add-apt-repository ppa:george-edison55/cmake-3.x
	
    sudo apt-get update
	
    sudo apt-get install cmake

2. Intall libusb 1.0

    sudo apt-get install libusb-1.0-0-dev

3. Install boost
   
    sudo apt-get install libboost-all-dev

4. Install doxygen

    sudo apt-get install flex
	
    sudo apt-get install bison
	
    Intstall livvib 
        
        wget http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.11.tar.gz
		
        tar -xvzf libiconv-1.11.tar.gz
		
        cd libiconv-1.11
        
        ./configure --prefix=/usr/local/libiconv
        
        make
        
        sudo make install
        
    git clone https://github.com/doxygen/doxygen.git
        
    cd doxygen
    
    mkdir build
    
    cd build
    
    cmake -G "Unix Makefiles" ..
    
    make
	
	make install

Next install libftd itself as follow:

    download the library from https://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.2.tar.bz2 and extract it whereever you want.
    cd libftdi folder
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="/usr" ../

	The output should look like this
	
	-- Build type: RelWithDebInfo
	
	-- Boost version: 1.58.0
	
	-- Found Doxygen: /usr/local/bin/doxygen (found version "1.8.12") 
	
	-- Doxygen found.
	
	-- Generating API documentation with Doxygen
	fatal: Not a git repository (or any of the parent directories): .git
	
	-- Detected git snapshot version: unknown
	
	-- Building libftdi1++
	
	-- Could NOT find SWIG (missing:  SWIG_EXECUTABLE SWIG_DIR) 
	
	-- Not building python bindings
	
	-- Could NOT find Confuse (missing:  CONFUSE_LIBRARY CONFUSE_INCLUDE_DIR) 
	
	-- libConfuse not found, won't build ftdi_eeprom
	
	-- Building example programs.
	
	-- Building libftdi++ examples.
	
	-- Boost version: 1.58.0
	
	-- Found the following Boost libraries:
	
	--   unit_test_framework
	
	-- Building unit test
	
	-- Configuring done
	
	-- Generating done
	
	-- Build files have been written to: /home/hessam/Desktop/libftdi1-1.2/build

    make

    make install

Part B:

Install OpenCV 

    sudo apt-get install libopencv-dev

Install Libstrophe 0.8.8-2
	
    sudo apt-get update
	
	sudo apt-get upgrade
	
	Using your favorite text editor (with root permision) modify /etc/apt/sources.list and replace jessie with stretch
	
	sudo apt-get update
	
	sudo apt-get install libstrophe-dev 
	
	Using your favorite text editor (with root permision) modify /etc/apt/sources.list and replace stretch with jessie

Parc C:

    cd ../sc-occdet_ir/build

    cmake ../

    make

    sudo ./HotSpotter