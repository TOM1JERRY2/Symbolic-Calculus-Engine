# Stage 1: Build the C++ Crow Web Server
FROM ubuntu:22.04 AS builder

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install necessary build tools, compilers, and library dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the entire repository into the build container
COPY . .

# Fix: Clean up local build artifacts and build completely fresh from the true root directory
RUN rm -rf build && rm -f CMakeCache.txt && rm -rf CMakeFiles
RUN mkdir -p build_dir && cd build_dir && cmake -DCMAKE_BUILD_TYPE=Release .. && make

# Stage 2: Create a minimal, lightweight runtime image
FROM ubuntu:22.04

WORKDIR /app

# Copy the compiled 'Calculator' binary from the fresh build directory stage
COPY --from=builder /app/build_dir/Calculator /app/Calculator

# Crow framework's default port. Change this if you customized it in Server.cpp
EXPOSE 18080

# Run the web server
CMD ["./Calculator"]
