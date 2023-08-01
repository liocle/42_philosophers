# Philosophers Project

![Philosophers Logo](link_to_logo.png)

## Table of Contents
- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Command-line Arguments](#command-line-arguments)
- [Function Descriptions](#function-descriptions)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Introduction
The Philosophers Project is a multi-threaded simulation of the classic dining philosophers problem. It was developed as a student project for School 42 (Hive Helsinki) and aims to demonstrate synchronization and mutual exclusion techniques using pthreads and mutexes.

## Requirements
- C Compiler (e.g., GCC, Clang)
- POSIX Threads (pthread library)

## Installation
1. Clone the repository: `git clone https://github.com/liocle/philosophers.git`
2. Navigate to the project directory: `cd philosophers`
3. Compile the program: `make`

## Usage
1. After compilation, execute the program with appropriate command-line arguments.
2. The program will simulate philosophers' dining behavior according to the specified parameters.

## Command-line Arguments
The program accepts the following command-line arguments:
- `number_of_philosophers`: The number of philosophers (1 or more).
- `time_to_die`: The time in milliseconds after which a philosopher dies if not eating.
- `time_to_eat`: The time in milliseconds it takes for a philosopher to eat.
- `time_to_sleep`: The time in milliseconds the philosopher spends sleeping.
- `[number_of_times_each_philosopher_must_eat]` (optional): The number of meals each philosopher must eat before the simulation stops (leave empty to run indefinitely).

Example: `./philo 5 800 200 150`

## Function Descriptions
- `parse_args`: Parses and validates command-line arguments.
- `prepare_party`: Initializes the philosopher party and necessary resources.
- `start_philosopher`: Creates a new philosopher thread.
- `start_monitoring`: Creates the monitoring thread to check for philosopher status.
- `philosopher_routine`: The routine executed by each philosopher thread.
- `monitoring_routine`: The routine executed by the monitoring thread.
- `clean_up`: Frees resources and terminates the program gracefully.

## Documentation
Detailed function descriptions and implementation details can be found in the source code.

## Contributing
Contributions are welcome! If you find any issues or want to improve the project, feel free to open a pull request or submit an issue.

## License
This project is licensed under the MIT License. As a student project, it's essential to respect any specific licensing requirements or obligations set by School 42.

## Acknowledgements
Special thanks to the contributors and developers of the philosophers project.
