<<<<<<< HEAD
# Install script for directory: /home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
=======
# Install script for directory: C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/main")
>>>>>>> myorigin/master
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
<<<<<<< HEAD
  set(CMAKE_OBJDUMP "/home/dnk133/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20240530/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump")
=======
  set(CMAKE_OBJDUMP "C:/Users/DNK133/.espressif/tools/xtensa-esp-elf/esp-13.2.0_20240530/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump.exe")
>>>>>>> myorigin/master
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
<<<<<<< HEAD
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
=======
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
>>>>>>> myorigin/master
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
<<<<<<< HEAD
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/home/dnk133/esp/v5.3.1/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
=======
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "C:/Users/DNK133/ESP/v5.3/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
>>>>>>> myorigin/master
    )
endif()
