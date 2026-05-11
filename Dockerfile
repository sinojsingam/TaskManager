# Use the official Ubuntu image as the base image
FROM ubuntu:latest

# Set oatpp dir
WORKDIR /oat

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    libcpprest-dev \
    libboost-all-dev \
    libssl-dev \
    git \
    cmake

# Compile oatpp
RUN git clone https://github.com/oatpp/oatpp.git &&  \
    cd oatpp/ &&  \
    mkdir build && \
    cd build/ && \
    cmake .. &&  \
    make install

# Set the working directory in the container
WORKDIR /app

# Copy the source code into the container
COPY . .

# delete build folder
RUN rm -rf build/

# compile the app
RUN mkdir build && \
    cd build/ && \
    cmake .. &&  \
    make

# Expose the port on which the API will listen
EXPOSE 8000

# Command to run the API when the container starts
CMD ["./build/TaskManager"]
