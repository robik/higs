vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/pfr
    REF boost-${VERSION}
    SHA512 93f7be2c02d255c581748ecdef95a8472f60cf2e14647c9d21204f1740a1eb34b3fc9b2fa9cdfd9f261b3b6f9cf406d82b09b0cd1ff2ce6ac7b80de645be7a86
    HEAD_REF master
)

set(FEATURE_OPTIONS "")
boost_configure_and_install(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
)