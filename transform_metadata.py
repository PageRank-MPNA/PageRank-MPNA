import numpy as np
import json

d = dict()
col = 0
with open("PageRank-MPNA/id_product_list.csv", "r") as file:
	for line in file:
		d[line[:-1]] = col
		col += 1

# ind = 0
# for dasin in d:
# 	if ind == 10:
# 		break
# 	print(dasin)
# 	ind+=1

data = []
i = 0
with open("correct_metadata.json", "r") as fr:
	with open("data_id.txt", "w") as fw:
		for line in fr:
			strl = ""
			if i%100000 == 0:
				print(i)
			l = json.loads(line)
			fw.write(str(d[l["asin"]])+" ")
			if "related" in l and "also_bought" in l["related"]:
				for asin in l["related"]["also_bought"]:
					if asin in d:
						fw.write(str(d[asin])+" ")
			# if "related" in data[i] and "also_viewed" in data[i]["related"]:
			# if "related" in data[i] and "bought_together" in data[i]["related"]:
			# if "related" in data[i] and "buy_after_viewing" in data[i]["related"]:
			fw.write("\n")
			# if i == 1000000:
			# 	break
			i+=1

