FROM ubuntu:22.04

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive

COPY PcntQSign-ubuntu-latest-x64.zip .

# 安装 Linux QQ
RUN arch=$(arch | sed s/aarch64/arm64/ | sed s/x86_64/amd64/) && \
    curl -o linuxqq.deb https://dldir1.qq.com/qqfile/qq/QQNT/63c751e8/linuxqq_3.2.15-30899_${arch}.deb && \
    dpkg -i --force-depends linuxqq.deb && rm linuxqq.deb

# 安装 PcntQSign
RUN arch=$(arch | sed s/aarch64/arm64/ | sed s/x86_64/x64/) && \
    unzip PcntQSign-ubuntu-latest-${arch}.zip && \
    chrom +x start.sh && \
    apt-get update && apt-get install -y \
    libnss3 \   
    libnotify4 \
    libsecret-1-0 \
    libgbm1 \
    libasound2 \
    fonts-wqy-zenhei \
    gnutls-bin \ 
    libglib2.0-dev \
    libdbus-1-3 \
    libgtk-3-0 \
    libxss1 \
    libxtst6 \
    libatspi2.0-0 \
    libx11-xcb1 \
    ffmpeg \
    unzip \
    curl && \   
    apt autoremove -y && \
    apt clean && \
    rm -rf \
    /var/lib/apt/lists/* \
    /tmp/* \
    /var/tmp/*

ENTRYPOINT ["sh", "start.sh"]

