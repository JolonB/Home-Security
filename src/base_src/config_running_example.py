import logging

# General operation mode. Uses logging enum types for simplicity
OP_MODE = logging.DEBUG  # change this if desired

logging.basicConfig(
    level=OP_MODE,
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        # logging.FileHandler("debug.log"), # for logging to file
        logging.StreamHandler()
    ],
)
