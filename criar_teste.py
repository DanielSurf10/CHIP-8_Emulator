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


write_integer_as_binary("teste", 0x650422061204220C750100EE750300EE)

# Assembly				| Opcode (hex)

# LD	V5 4	0x200	| 6504
# CALL	func	0x202	| 2206
# JP	0x204	0x204	| 1204
# func:					|
# CALL	outra	0x206	| 220C
# ADD	V5 1	0x208	| 7501
# RET			0x20A	| 00EE
# outra:				|
# ADD	V5 3	0x20C	| 7503
# RET			0x20E	| 00EE
