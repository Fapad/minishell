FROM ubuntu:latest
RUN apt-get update -y
RUN apt-get install -y build-essential readline-common libreadline-dev libreadline7

WORKDIR /app
COPY . /app
RUN make

CMD ["./minishell"]