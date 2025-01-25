<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- ABOUT THE PROJECT -->
## About 

I've been itching to write something non-trivial in C, and this is my excuse to do so. The aim of this project
is to vastly improve my C programming skills, with an additional stretch goal of implementing an HTTP/1.1-compliant
server.

As much as possible, I will be attempting to implement functionality without the use of external libraries. This is not 
absolutely feasible as, for example, I will undoubtedly rely on the OpenSSL project to implement HTTPS functionality (although rolling
crypto on my own is something I'd love to attempt separately one day).

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

At the moment, a Unix-compliant system is required for building and running the server. In the future, I may extend upon my development 
process to attempt to ensure full cross-platform capabilities.

### Setup

1. Clone the repo
   ```sh
   git clone https://github.com/wytata/http-easy.git
   ```
2. Build the server executable
   ```sh
   make
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- USAGE EXAMPLES -->
## Usage

At the moment, there is very little functionality to the server. Nevertheless, you may simply run the server on your machine as follows:
```sh
./http-easy
```

<!-- ROADMAP -->
## Roadmap

- [ ] Actually implement this roadmap
