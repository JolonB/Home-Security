import logging

logging.basicConfig(
    level=logging.DEBUG, # change this if desired
    format="%(asctime)s [%(levelname)s] %(message)s",
    handlers=[
        # logging.FileHandler("debug.log"), # for logging to file
        logging.StreamHandler()
    ]
)