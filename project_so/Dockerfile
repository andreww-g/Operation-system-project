FROM ubuntu:22.04
LABEL authors="andrii"
# Zainstaluj potrzebne pakiety
RUN apt-get update && \
    apt-get install -y cmake && \
    apt-get install -y libncurses5-dev && \
    apt-get install -y gcc g++ && \

    rm -rf /var/lib/apt/lists/*

# Skopiuj kod źródłowy do obrazu
COPY . /app

# Ustaw katalog roboczy na katalog /app
WORKDIR /app

# Skompiluj kod źródłowy przy użyciu cmake
RUN cmake . && \
    make

# Uruchom aplikację
CMD ["./project_so", "5"]
