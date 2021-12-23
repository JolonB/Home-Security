import re
import math
import subprocess
import netifaces as ni

from getmac import get_mac_address

IP_REGEX = re.compile(r"Host: (\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}) \(\)\tStatus: Up\n")


def get_local_ip() -> tuple:
    """Get the local IP address and netmask.

    Raises:
        RuntimeError: If no valid network interface is found.

    Returns:
        tuple: A tuple containing the local IP address and netmask.
    """
    # Find all of the possible network interfaces
    interfaces = ni.interfaces()
    for interface in interfaces:
        # Skip localhost interface
        if interface == "lo":
            continue

        # If the interface doesn't have valid information, skip it
        iface = ni.ifaddresses(interface).get(ni.AF_INET)
        if iface:
            mask = int(
                sum(
                    [
                        math.log2(int(subnet) + 1)
                        for subnet in iface[0]["netmask"].split(".")
                    ]
                )
            )
            return iface[0]["addr"], mask
    raise RuntimeError("No valid interface found")


def get_local_devices(keys: str = "mixed", return_failed: bool = False) -> dict:
    """Get the IP and MAC addresses of all devices on the local network.

    Args:
        keys (str, optional): The keys to return in the dict. Can be `mixed`,
            `ip`, or `mac`. If `mixed`, the dict will be at most twice the size
            with key-value pairs of {ip:mac} and {mac:ip}. If `ip`, the dict
            only contains {ip:mac} pairs. Defaults to 'mixed'.
        return_failed (bool, optional): Whether to include IP addresses with no
            MAC address in the dict. Ignored if `keys` is `mac`. Defaults to
            False.

    Raises:
        ValueError: If `keys` is not one of `mixed`, `ip`, or `mac`.

    Returns:
        dict: A dict of IP addresses and MAC addresses.
    """
    # Validate the `keys` argument
    if keys not in ("mixed", "ip", "mac"):
        raise ValueError("Invalid keys argument. Must be 'mixed', 'ip' or 'mac'")

    # Get local IP and mash
    local_ip, netmask = get_local_ip()

    # Run nmap to get all the local IP addresses
    result = subprocess.run(
        ["nmap", "-sn", "{}/{}".format(local_ip, netmask), "-n", "-oG", "-"],
        capture_output=True,
    )
    result = result.stdout.decode("utf-8")
    ip_addresses = IP_REGEX.findall(result)

    devices = {}

    for addr in ip_addresses:
        # Skip localhost
        if addr == local_ip:
            continue

        # Get the MAC address of the device
        mac = get_mac_address(ip=addr, network_request=True)
        if mac:
            if keys == "mixed" or keys == "ip":
                devices[addr] = mac
            if keys == "mixed" or keys == "mac":
                devices[mac] = addr
        elif return_failed:
            if keys == "mixed" or keys == "ip":
                devices[addr] = None

    return devices
