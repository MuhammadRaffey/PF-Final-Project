# Final Project

## Table of Contents

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [Installaion](#installation)
- [Usage](#usage)
- [Files](#files)
- [License](#license)

## Introduction

This is the final project for the Programming Fundamentals 4 course. The project aims to demonstrate the skills and knowledge acquired throughout the semester.

## Prerequisites

- Docker
- Docker Compose
- g++
- libcurl
- OpenAI API Key (stored in `.env` file)

## Installation

To install the project, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/final-project.git
   ```
2. Navigate to the project directory:
   ```bash
   cd final-project
   ```
3. Build the Docker container:
   ```bash
   docker-compose up --build
   ```

## Usage

To run the project, use the following command:

1. **Run the application:**

   ```sh
   docker exec -it cpp_project_container bash
   g++ -o main main.cpp -lcurl -I.
   ./main
   ```

2. **Interact with the application:**
   - Choose option 1 to ask a question.
   - Choose option 2 to view saved interactions.
   - Choose option 3 to exit the application.

## Files

- **main.cpp:** Contains the main logic of the application.
- **dotenv.h:** Utility for loading environment variables from a [.env](http://_vscodecontentref_/6) file.
- **json.hpp:** JSON library used for parsing API responses.
- **Dockerfile:** Docker configuration for setting up the environment.
- **docker-compose.yml:** Docker Compose configuration for managing the container.
- **.vscode/:** VSCode configuration files for the project.

## License

This project is licensed under the MIT License.
