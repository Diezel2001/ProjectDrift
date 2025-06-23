FROM arm64v8/debian:bookworm

# Install dependencies for C++ and Python dev
RUN apt-get update && apt-get install -y \
    python3 python3-pip python3-dev \
    build-essential cmake git curl \
    libffi-dev libssl-dev zlib1g-dev \
    libbz2-dev libreadline-dev libsqlite3-dev \
    libncursesw5-dev libgdbm-dev libnss3-dev \
    liblzma-dev uuid-dev tk-dev \
    libexpat1-dev libdb-dev

# Optional: symlink python3 to python if you want `python` to work
RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 1 && \
    python --version && python3 --version

# Set working directory
WORKDIR /project

CMD ["bash"]
