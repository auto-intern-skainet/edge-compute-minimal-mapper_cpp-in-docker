# Multi-stage Dockerfile for C++ Redis timer
FROM alpine:latest AS builder

# Install build dependencies
RUN apk add --no-cache \
    cmake \
    make \
    g++ \
    git \
    hiredis-dev

WORKDIR /build

# Copy source
COPY CMakeLists.txt .
COPY src/ src/

# Build
RUN cmake -S . -B build && \
    cmake --build build --target main -- -j$(nproc)

# Runtime stage
FROM alpine:latest

# Install runtime dependencies
RUN apk add --no-cache hiredis

# Copy binary
COPY --from=builder /build/build/main /usr/local/bin/main

# Run
CMD ["/usr/local/bin/main"]