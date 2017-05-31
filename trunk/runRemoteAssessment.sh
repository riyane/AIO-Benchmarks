#!/bin/bash



function localRequest
{
	make plotPointCompareHPC
}


function remoteRequest
{
	make runAllBenchmarkHPC
}






ssh_remote_id=sidlakhr
ssh_remote_domain=jureca.fz-juelich.de

git_commitTitle="Automatic commit: in order to run tests on HPC_"`date "+%F-%T"`
git_url=github.com/riyane/AIO-Benchmarks.git
git_protocol=https
git_uname=riyane
git_pwd=bidonB1

remote_pathResult=AIO-Benchmarks/resource/statistic/
remote_pathRun=AIO-Benchmarks/trunk/
local_pathResult=../resource/statistic/


function sendRequest
{
	make mrproper
	git add -A
	git commit -m \""$git_commitTitle"\"
	git pull
	git push $git_protocol://$git_uname:$git_pwd@$git_url

	echo; echo; echo; echo; echo; echo; echo; echo; 
	echo "-----------------------------"
	echo "Start the remote execution"
	echo "-----------------------------"
	ssh ${ssh_remote_id}@${ssh_remote_domain} "bash -s -l" -- < ${0} "remote"

	echo; echo; echo; echo; echo; echo; echo; echo; 
	echo "-----------------------------"
	echo "End the remote execution"
	echo "-----------------------------"
	scp ${ssh_remote_id}@${ssh_remote_domain}:"~"/${remote_pathResult}/* ${local_pathResult}
	localRequest
}


function processRemoteRequest
{
	source ~/.bashrc
	cd ${remote_pathRun}
	rm ../resource/statistic/*
	make mrproper
	git pull
	remoteRequest
}



if [ ${#} -eq "1" ] && [ ${1} == remote ]; then
	processRemoteRequest
else
	sendRequest
fi




