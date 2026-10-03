FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libasio-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# CRITICAL FIX: Make a copy so BOTH 'ASTNode.hpp' and 'ASTnode.hpp' exist simultaneously!
RUN cp ASTnode.hpp ASTNode.hpp

RUN rm -f CMakeCache.txt && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

FROM ubuntu:22.04

WORKDIR /app

COPY --from=builder /app/build/Calculator /app/Calculator

EXPOSE 18080

CMD ["./Calculator"]
