# Extapathy:
# it's seems like an odd system but I think the reasoning is this:
# the first 4 bits are always zero
# rot4 represents the first 2 bits of the decimal part of the value for x/y/z
# rot1/rot2/rot3 represent the next 8 decimal bits the value for x/y/z in a byte
# the reason for the shift left 2 is to move the byte to the right position
# and the reason for the shift 10 is to move the rot4 bits to the right position
# o this is kinda inherited from the GTE that the playstation uses, the representation
# of decimals (because it doesn't use floating point) is 1 bit for the sign, 3 bits for the integral part
# and 12 bits for the decimal part and that's just stored in a short here instead because you can't
# natively use fixed point types in C# instead, the values are likely converted to a float and divided by
# 4096 (2^12) before they are used
# it looks like what you end up with is: 0b0000'XXNN'NNNN'NN00 XX are the bits from r4,
# and NNNNNNNN are bits from r1_3
import numpy, struct


def convert_byte_pair_to_int16(r1_3, r4, offset):
    return numpy.int16((r1_3 << 2) | (((r4 >> (2 * offset)) & 3) << 10))


def convert_byte_set_to_int16_set(r1, r2, r3, r4):
    return list(map(lambda x: convert_byte_pair_to_int16(x[1], r4, x[0]), enumerate((r1, r2, r3))))


def convert_four_bytes_to_int16_set(fourbytes):
    return convert_byte_set_to_int16_set(fourbytes[0], fourbytes[1], fourbytes[2], fourbytes[3])


def convert_int16_set_to_float_set(int16_set):
    return list(map(lambda x: (float(x) * 360.0) / 4096.0, int16_set))


def test():
    for i in range(0, 0xFF):
        int_set = convert_four_bytes_to_int16_set([i, i, i, 0x34])
        # 0x34 = 0b110100 covers all the values those 2 bits can have.
        print(int_set)
        print(convert_int16_set_to_float_set(int_set))
    pass


#test()


mdl_offset = 16928
animation_data_offset = 46644


def read_mch(mch_path, offset):
    with open(mch_path, 'rb') as f:
        f.seek(offset)

        
read_mch("p043.mch", mdl_offset+animation_data_offset)