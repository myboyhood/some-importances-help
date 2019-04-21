#!/usr/bin/python


fmarker = open("fmarker.txt")
fcamera = open("fcamera.txt")
fselect = open("fselect.txt","a+")



linemarks = fmarker.readlines()
for linecam in fcamera.readlines():
	flag = 0
	split_str_cam = linecam.split()
	time_str_cam = split_str_cam[0]
	time_str_cam = time_str_cam[7:]
	time_cam = float(time_str_cam)
	time_cam = round(time_cam,6)
	error_least = 10	
	

	for linemark in linemarks:
		split_str_mark = linemark.split()
		time_str_mark = split_str_mark[0]
		time_str_mark = time_str_mark[7:]
		time_mark = float(time_str_mark)
		time_mark = round(time_mark,6)		
		error = time_mark - time_cam
		error = round(error,6)
		if(abs(error) <= abs(error_least)):
			error_least = error
			error_least = round(error_least,6)
	print error_least


	for linemark in linemarks:
		split_str_mark = linemark.split()
		time_str_mark = split_str_mark[0]
		time_str_mark = time_str_mark[7:]
		time_mark = float(time_str_mark)
		time_mark = round(time_mark,6)		
		error = time_mark - time_cam
		error = round(error,6)	
	
		if(abs(error) <= abs(error_least)) & (flag == 0):
			fselect.writelines([split_str_mark[0]," ",split_str_mark[1]," ",split_str_mark[2]," ",split_str_mark[3],"\n"])
			flag = 1
			print time_cam


	

fselect.close()
fmarker.close()
fcamera.close()


			


