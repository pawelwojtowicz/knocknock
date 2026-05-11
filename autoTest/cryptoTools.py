from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding
from cryptography.hazmat.backends import default_backend
import hashlib

def aes_cbc_encrypt(plaintext: str, iv_hex: str, key_hex: str) -> str:
    """
    AES-CBC encrypt plaintext using hex-encoded IV and key.
    Returns hex-encoded ciphertext (matching the C++ EncryptString behavior).
    """
    iv_bytes = bytes.fromhex(iv_hex)
    key_bytes = bytes.fromhex(key_hex)

    if len(key_bytes) != 32:
        raise ValueError(f"AES-256 requires a 32-byte key. Got {len(key_bytes)} bytes.")

    if len(iv_bytes) == 32:
        iv_bytes = iv_bytes[:16]
    elif len(iv_bytes) != 16:
        raise ValueError(f"AES-CBC requires a 16-byte IV. Got {len(iv_bytes)} bytes.")

    # PKCS7 padding (OpenSSL default)
    padder = padding.PKCS7(128).padder()
    padded_data = padder.update(plaintext.encode("utf-8")) + padder.finalize()

    cipher = Cipher(algorithms.AES(key_bytes), modes.CBC(iv_bytes))
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(padded_data) + encryptor.finalize()

    return ciphertext.hex().upper()

def generate_sha256(text: str) -> str:
    """
    Generate the SHA-256 hash of a string.

    Args:
        text: The input string to hash.

    Returns:
        Hexadecimal digest of the SHA-256 hash.
    """
    return hashlib.sha256(text.encode('utf-8')).hexdigest().upper()