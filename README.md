# Mini Visual Lib

A small embedded graphics library for OLED displays, built for ESP32 projects.

## Overview

Mini Visual Lib is a lightweight graphics library for small OLED displays. The goal of the project is to create reusable visual components for embedded systems, instead of rewriting the same display code again and again.

The library includes simple UI elements such as icons, progress bars, spinners, sparklines, and small animations that can be used in ESP32-based projects.

## Features

- OLED display graphics
- Icons and small visual indicators
- Progress bars
- Loading spinners
- Sparkline-style mini graphs
- Simple animation helpers
- Reusable drawing functions
- Wokwi simulation support

## Tech Stack

- ESP32
- C / C++
- OLED display
- Wokwi
- Embedded graphics programming

## Why I built this

Small embedded displays are usually used for basic text output, but I wanted to make them more expressive.

Instead of only printing values to the screen, I wanted a reusable graphics layer that could display information more clearly using small visual components.

This project helped me understand display rendering, pixel-level layout, embedded UI design, and how to structure reusable graphics code for microcontroller projects.

## Project Files

```text
main.ino       - Demo sketch showing the graphics library in use
MiniGFX.h     - Graphics function declarations
MiniGFX.cpp   - Graphics function implementations
libraries.txt    - Wokwi library dependencies
