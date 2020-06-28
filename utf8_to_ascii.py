import os
current_dir = os.path.dirname(os.path.realpath(__file__))

# read from utf-8 encoded txt file and decode to str
plain_text_f = open((current_dir + '/utf-8.txt'), 'r', encoding = 'utf-8')
plain_text = plain_text_f.read()
print (plain_text)

# convert str to ascii
ascii_text = plain_text.encode("ascii","ignore")
print (ascii_text)
plain_text_f.close()    

# write ascii to txt file (ascii encoded txt file)
ascii_text_f = open((current_dir + '/ascii.txt'), 'wb')
ascii_text_f.write(ascii_text)
ascii_text_f.close()