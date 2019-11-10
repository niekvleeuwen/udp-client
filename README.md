# udp-client
Features
+ Send a struct over UDP
+ Change the interval with an argument

## Usage

Example usage

```bash
./upd-client --serverip 164.93.146.29 --port 7755 --interval 5
```

Available options
```
-si / --serverip        IP address of the server to which the packets are sent 
                        (default: 127.0.0.1)

-p / --port             Port of the server to which the packets are sent (0-65536) 
                        (default: 7755)

-i / --interval         The interval at which the packets are sent (in microseconds)
                        (default: 1000)

-h / --help             Show this information
```

## Contributing
Everyone is welcome to create an issues in case of problems or feature request. You can also submit a pull request.
