# attackiq-dnp3-client
Simple client (master) application based on opendnp3 that takes command line arguments

# building

1) Build install the tip of opendnp3 2.0.x series

2) Checkout this repository recursively, as git submodules are used for dependencies:

```
> git clone --recursive <repo address>
```

3) Build using cmake/make

```
> mkdir build; cd build
> cmake ..
> make
```


