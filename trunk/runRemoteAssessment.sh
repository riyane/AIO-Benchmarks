#!/bin/bash



function localRequest
{
	make plotWriteTime
}


function remoteRequest
{
	make runWriteTimeBenchmark
}






ssh_remote_id=sidlakhr
ssh_remote_domain=jureca.fz-juelich.de

git_commitTitle="Automatic commit: in order to run tests on HPC_"`date "+%F-%T"`
git_url=github.com/riyane/AIO-Benchmarks.git
git_protocol=https
git_uname=riyane
git_pwd=bidonB1

remote_path=~/AIO-Benchmarks
remote_pathResult=${remote_path}/resource/statistic/
remote_pathRun=${remote_path}/trunk/
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
	ssh ${ssh_remote_id}@${ssh_remote_domain} "bash -s" -- < ${0} "remote"
#	scp ${ssh_remote_id}@${ssh_remote_domain}:~/${remote_pathResult}/* ${local_pathResult}
#	localRequest
}


function processRemoteRequest
{
	cd ${remote_pathRun}
	git pull
	source ~/.bashrc
	make mrproper
make posixGlibcIO_sleep; ./posixGlibcIO_sleep
#	remoteRequest
}



if [ ${#} -eq "1" ] && [ ${1} == remote ]; then
	processRemoteRequest
else
	sendRequest
fi




