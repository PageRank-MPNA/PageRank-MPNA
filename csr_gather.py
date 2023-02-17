import numpy as np

i = 0
j = 0
k = 0
temp = 0
maxi = 200
with open("sparse_metadata_piece.txt", "w") as fw:
	fw.write("105891595 9430088 105891595\n")
	with open("csc_line.txt", "r") as file1:
		for line in file1:
			fw.write(str(line[:-1]))
			break
		for line in file1:
			fw.write(" "+str(line[:-1]))
			if i == maxi:
				break
			i+=1
		fw.write("\n")
			
	with open("csc_col.txt", "r") as file2:
		for line in file2:
			fw.write(str(line[:-1]))
			break
		for line in file2:
			fw.write(" "+str(line[:-1]))
			if j == maxi:
				break
			j+=1
		fw.write("\n")

	with open("csc_col.txt", "r") as file2:
		for line in file2:
			ecart = int(line) - temp
			if ecart == 0:
				continue
			for it in range(ecart):
				fw.write(str(1./ecart)[:8])
				if it != ecart-1:
					fw.write(" ")
			temp = int(line)
			break
		for line in file2:
			ecart = int(line) - temp
			if ecart == 0:
				continue
			for it in range(ecart):
				fw.write(" "+str(1./ecart)[:8])
			temp = int(line)
			if k == 35:
				break
			k+=1
		fw.write("\n")
			