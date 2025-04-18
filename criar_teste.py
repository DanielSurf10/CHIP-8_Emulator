def write_integer_as_binary(file_name, number, byte_order="big"):
    """
    Escreve um número inteiro em um arquivo em formato binário.

    :param file_name: Nome write_string_as_binarydo arquivo a ser criado.
    :param number: Número inteiro a ser escrito.
    :param byte_order: Ordem dos bytes ("big" para big-endian, "little" para little-endian).
    """
    # Calcula o número de bytes necessários para representar o número
    num_bytes = (number.bit_length() + 7) // 8 or 1

    # Abre o arquivo em modo binário para escrita
    with open(file_name, "wb") as file:
        # Converte o número para bytes e escreve no arquivo
        file.write(number.to_bytes(num_bytes, byteorder=byte_order))


# write_integer_as_binary("teste", 0x650422061204220C750100EE750300EE)

# Teste CALL e RET
# Assembly			| addr	| Opcode (hex)
# ------------------|-------|-------------
# LD	V5, 4		| 0x200 | 6504
# CALL	func		| 0x202 | 2206
# JP	0x204		| 0x204 | 1204
# func:				| 		|
# CALL	outra		| 0x206 | 220C
# ADD	V5, 1		| 0x208 | 7501
# RET				| 0x20A | 00EE
# outra:			| 		|
# ADD	V5, 3		| 0x20C | 7503
# RET				| 0x20E | 00EE


# Teste SE e SNE
write_integer_as_binary("teste", 0x6005410061013200120C121062001212620193001218121C6300120273011202)

# Código
# V[0] = 5
#
# while True:
# 	if V[1] == 0
# 		V[1] = 1
#
# 	if V[2] != 0
# 		V[2] = 0
# 	else
# 		V[2] = 1
#
# 	if V[3] == V[0]
# 		V[3] = 0
# 	else
# 		V[3] += 1

# Assembly			| addr	| Opcode (hex)
# ------------------|-------|-------------
# LD	V0, 5		| 0x200 | 6005
# loop:				|		|
# SNE	V1, 0		| 0x202 | 4100
# LD	V1, 1		| 0x204 | 6101
# SE	V2, 0		| 0x206 | 3200
# JP	bloco1		| 0x208 | 120C
# JP	bloco2		| 0x20A | 1210
# bloco1:			|		|
# LD	V2, 0		| 0x20C | 6200
# JP	fim_if_1	| 0x20E | 1212
# bloco2:			|		|
# LD	V2, 1		| 0x210 | 6201
# fim_if_1:			|		|
# SNE	V3, V0		| 0x212 | 9300
# JP	bloco3		| 0x214 | 1218
# JP	bloco4		| 0x216 | 121C
# bloco3:			|		|
# LD	V3, 0		| 0x218 | 6300
# JP	loop		| 0x21A | 1202
# bloco4:			|		|
# ADD	V3, 1		| 0x21C | 7301
# JP	loop		| 0x21E | 1202


# Teste LD, OR, AND, XOR, ADD, SUB, SUBN, SHL, SHR
write_integer_as_binary("teste", 0x600F62FF63FF64FF6505660667046806690181008201830284038655865587578577890E8906)

# Assembly			| addr	| Opcode (hex)
# ------------------|-------|-------------
# LD	V0, 0F		| 0x200 | 600F
# LD	V2, FF		| 0x202 | 62FF
# LD	V3, FF		| 0x204 | 63FF
# LD	V4, FF		| 0x206 | 64FF
# LD	V5, 05		| 0x208 | 6505
# LD	V6, 06		| 0x20A | 6606
# LD	V7, 04		| 0x20C | 6704
# LD	V8, 06		| 0x20E | 6806
# LD	V9, 01		| 0x210 | 6901
#					|		|
# LD	V1, V0		| 0x212 | 8100
# OR	V2, V0		| 0x214 | 8201
# AND	V3, V0		| 0x216 | 8302
# XOR	V4, V0		| 0x218 | 8403
#					|		|
# ; VX = VX - VY	|		|
# SUB	V6, V5		| 0x21A | 8655		; vx > vy
# SUB	V6, V5		| 0x21C | 8655		; vx <= vy
#					|		|
# ; VX = VY - VX	|		|
# SUBN	V7, V5		| 0x21E | 8757		; vy > vx
# SUBN	V5, V7		| 0x220 | 8577		; vy <= vx
#					|		|
# SHL	V9, V0		| 0x222 | 890E
# SHR	V9, V0		| 0x224 | 8906
