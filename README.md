# Sit Back

## Introduction.
This project aims to protect you from forwarding posture by popping a window to remind you that you are too close to your screen.

There are two options/versions of this design.
- Software: It don't need extra hardware except your laptop. All workload will be done on your PC.
- Hardware: It is based on a Raspberry PI. There is no impact on your working environment.

## [Option 1] Software:
### Build

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Run

Click the `sitback.exe`


## [Option 2] Hardware:

### Build

There is no need to build since it is coded with Python.
Just install `python` and pip install the dependencies accordingly.
### Run


`python3 sitback.py`