# -------------------------------------------------------------
# Constants

# Generator polynomials
CRC_8_CCITT_POLY = 0x07
CRC_16_CCITT_POLY = 0x1021


# -------------------------------------------------------------
# Data

# Lookup tables are stored here. Tables are only
# initialised when the crc function requiring the
# table is first called
lookup_tables = {}


# -------------------------------------------------------------
# Public functions


def crc8_ccitt(byte_array):
    if 'crc8_ccitt' not in lookup_tables:
        lookup_tables['crc8_ccitt'] = get_crc8ccitt_lookup_table()
    crc = 0
    for i in range(len(byte_array)):
        table_idx = crc ^ byte_array[i]
        crc = lookup_tables['crc8_ccitt'][table_idx]
    return crc


def crc16_ccitt(byte_array):
    if 'crc16_ccitt' not in lookup_tables:
        lookup_tables['crc16_ccitt'] = get_crc16ccitt_lookup_table()
    crc = 0
    for i in range(len(byte_array)):
        table_idx = ((crc >> 8) ^ byte_array[i])
        crc = ((crc << 8) & 0xFFFF) ^ lookup_tables['crc16_ccitt'][table_idx]
    return crc


# -------------------------------------------------------------
# Utility functions


def get_crc8ccitt_lookup_table():
    """ Generate the lookup table for the 8-bit ccitt crc """
    table = []

    for i in range(256):
        crc = i

        for j in range(8):
            if crc & 0x80:
                crc = ((crc << 1) % 2 ** 8) ^ CRC_8_CCITT_POLY
            else:
                crc = (crc << 1) % 2 ** 8

        table.append(crc)

    return table


def get_crc16ccitt_lookup_table():
    """ Generate the lookup table for the 16-bit ccitt crc
        Method obtained from Lammert Bies:
        http://www.lammertbies.nl/comm/info/crc-calculation.html
    """
    table = []

    for i in range(256):
        crc = 0
        c = i << 8

        for j in range(8):
            if (crc ^ c) & 0x8000:
                crc = ((crc << 1) % 2 ** 16) ^ CRC_16_CCITT_POLY
            else:
                crc = (crc << 1) % 2 ** 16
            c = (c << 1) % 2 ** 16

        table.append(crc)

    return table


def get_table_as_c_array(table, element_numbits):
    """ Get a crc lookup table as a string in C array
        initialiser format
    """
    out_str = '{\n    '
    vals_on_line = 0
    for value in table:
        if element_numbits == 8:
            hex_val = '%02x' % value
        elif element_numbits == 16:
            hex_val = '%04x' % value
        else:
            raise Exception('Unsupported number of bits')
        out_str += '0x' + hex_val.upper() + ','
        vals_on_line += 1
        if vals_on_line == 8:
            out_str += '\n    '
            vals_on_line = 0
    out_str += '};'
    return out_str


if __name__ == '__main__':
    # The crcs calculated here agree with
    # http://depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm
    # and
    # http://ghsi.de/CRC/index.php?Polynom=10001000000100001&Message=3939
    print hex(crc8_ccitt(bytearray('0')))
    print hex(crc8_ccitt(bytearray('1')))
    print hex(crc8_ccitt(bytearray('10')))
    print hex(crc8_ccitt(bytearray('99')))

    # The crcs calculated here agree with
    # http://www.lammertbies.nl/comm/info/crc-calculation.html
    # and
    # http://depa.usst.edu.cn/chenjq/www2/SDesign/JavaScript/CRCcalculation.htm
    # and
    # http://ghsi.de/CRC/index.php?Polynom=10001000000100001&Message=3939
    print hex(crc16_ccitt(bytearray('0')))
    print hex(crc16_ccitt(bytearray('1')))
    print hex(crc16_ccitt(bytearray('10')))
    print hex(crc16_ccitt(bytearray('99')))
