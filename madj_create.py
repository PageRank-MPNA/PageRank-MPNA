import numpy as np
import json

def madj_also_bought(data):
	mat = np.eye(len(data))
	for i in range(len(data)):
		for j in range(len(data)):
			if "related" in data[i] and "also_bought" in data[i]["related"]:
				if data[j]["asin"] in data[i]["related"]["also_bought"]:
					mat[i][j] = 1;
	return mat

def madj_also_viewed(data):
	mat = np.eye(len(data))
	for i in range(len(data)):
		for j in range(len(data)):
			if "related" in data[i] and "also_viewed" in data[i]["related"]:
				if data[j]["asin"] in data[i]["related"]["also_viewed"]:
					mat[i][j] = 1;
	return mat

def madj_bought_together(data):
	mat = np.eye(len(data))
	for i in range(len(data)):
		for j in range(len(data)):
			if "related" in data[i] and "bought_together" in data[i]["related"]:
				if data[j]["asin"] in data[i]["related"]["bought_together"]:
					mat[i][j] = 1;
	return mat

def madj_buy_after_viewing(data):
	mat = np.eye(len(data))
	for i in range(len(data)):
		for j in range(len(data)):
			if "related" in data[i] and "buy_after_viewing" in data[i]["related"]:
				if data[j]["asin"] in data[i]["related"]["buy_after_viewing"]:
					mat[i][j] = 1;
	return mat


with open("piece_of_metadata.json", "r") as file:
	line = file.read()
	data = json.loads(line)

print("also_bought\n", madj_also_bought(data), "\n")
print("also_viewed\n", madj_also_viewed(data), "\n")
print("bought_together\n", madj_bought_together(data), "\n")
print("buy_after_viewing\n", madj_buy_after_viewing(data), "\n")


# with open("piecemetadata.json", "w") as file:
# 	for line in data:
# 		file.write(line)


