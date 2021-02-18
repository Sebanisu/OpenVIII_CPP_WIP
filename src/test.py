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
import numpy
import struct
import os


def convert_byte_pair_to_int16(r1_3, r4, offset):
    return numpy.int16((r1_3 << 2) | (((r4 >> (2 * offset)) & 3) << 10))


def convert_byte_set_to_int16_set(r1, r2, r3, r4):
    return list(map(lambda x: convert_byte_pair_to_int16(x[1], r4, x[0]), enumerate((r1, r2, r3))))


def convert_four_bytes_to_int16_set(fourbytes):
    return convert_byte_set_to_int16_set(fourbytes[0], fourbytes[1], fourbytes[2], fourbytes[3])


def convert_int16_set_to_float_set(int16_set):
    return list(map(lambda x: (float(x) * 360.0) / 4096.0, int16_set))


def make_int16_set_negative(int16_set):
    return list(map(lambda x: -x, int16_set))


def print_rot(rot_array):
    int_set = convert_four_bytes_to_int16_set(rot_array)
    # print(int_set)
    int_set = swap_x_y_from_int16_set(int_set)
    int_set = make_int16_set_negative(int_set)
    print(int_set)
    # 0x34 = 0b110100 covers all the values those 2 bits can have.
    print(convert_int16_set_to_float_set(int_set))
    pass


def swap_x_y_from_int16_set(int_set):
    return [int_set[1], int_set[0], int_set[2]]  # swap x and y


def test():
    for i in range(0, 0xFF):
        print_rot([i, i, i, 0x34])
    pass


# test()

def read_tim_offsets(f):
    while True:
        tim_offset = struct.unpack('<I', f.read(4))[0]
        if tim_offset == 0xffffffff:
            break
        else:
            yield tim_offset
    pass


def read_mch(mch_path):
    if os.stat(mch_path).st_size < 64:  # skip dummy files.
        return
    with open(mch_path, 'rb') as f:
        tim_offsets = [tim_off for tim_off in read_tim_offsets(f)]
        print(tim_offsets)
        model_offset = struct.unpack('<I', f.read(4))[0]
        f.seek(model_offset, 0)
        print(model_offset)
        header = struct.unpack('<7I2H8I', f.read(64))
        # print(header)
        (cSkeletonBones, cVertices, cTexAnimations, cFaces, cUnk, cSkinObjects, Unk, cTris, cQuads, pBones, pVertices,
         pTexAnimations, pFaces, pUnk, pSkinObjects, pAnimation, Unk2) = header
        print(pAnimation)
        f.seek(model_offset + pAnimation, 0)
        animations_count = struct.unpack('<H', f.read(2))[0]
        print('Animations Count:' + str(animations_count))
        for animation in range(animations_count):
            print('#### Animation N.' + str(animation) + ' ####')
            animation_frames = struct.unpack('<H', f.read(2))[0]
            animation_bones = struct.unpack('<H', f.read(2))[0]
            print('Animations Frames:' + str(animation_frames))
            print('Animations Bones:' + str(animation_bones))
            for frame in range(animation_frames):
                print('#### Frame: ' + str(frame))
                key_point = struct.unpack('<3h', f.read(6))
                print(key_point)
                for bone in range(animation_bones):
                    print('#### Bone:')
                    rot = struct.unpack('<4B', f.read(4))  # unsigned char
                    print(rot)
                    print_rot(rot)
    pass


def all_mch(path):
    for file_path in walk_path_for_ext(path, ".mch"):
        print(file_path)
        read_mch(file_path)
    pass


# mdl_offset = 16928
# animation_data_offset = 46644
# read_mch("p043.mch", mdl_offset + animation_data_offset)
# read_mch("d043.mch")
# all_mch(r'D:\dev\OpenVIII_CPP_WIP\cmake-build-debug\Tests\src\tmp\ff8\data\eng\FIELD\model\main_chr')
# all_mch(r'D:\dev\OpenVIII_CPP_WIP\cmake-build-debug\Tests\src\tmp\ff8\data\x\field\model\main_chr')

def walk_path_for_ext(path, extension):
    for root, dirnames, filenames in os.walk(path):
        for filename in filenames:
            if filename.endswith(extension):
                yield os.path.join(root, filename)
    pass


def all_one(path):
    for file_path in walk_path_for_ext(path, ".one"):
        print(file_path)
    pass


all_one(r'D:\dev\OpenVIII_CPP_WIP\cmake-build-debug\Tests\src\tmp\ff8\data\eng\field')
all_one(r'D:\dev\OpenVIII_CPP_WIP\cmake-build-debug\Tests\src\tmp\ff8\data\x\field')
