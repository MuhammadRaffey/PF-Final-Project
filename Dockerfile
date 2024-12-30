# Use an official Ubuntu image
FROM ubuntu:22.04

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary tools and libraries
RUN apt-get update && apt-get install -y \
    g++ \
    libcurl4-openssl-dev \
    curl \
    git \
    wget \
    make \
    build-essential && \
    apt-get clean

# Set the working directory in the container
WORKDIR /app

# Copy project files into the container
COPY . .

# Command to build and run the project
CMD ["bash"]
