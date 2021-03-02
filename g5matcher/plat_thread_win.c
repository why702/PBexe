#include <windows.h>
#include "plat_thread.h"
//#include "biosign_lib.h"
#include "plat_log.h"



int plat_thread_create_ex(thread_handle_t* handle, void* routine, thread_param_t* arg)
{
	if (handle == NULL || routine == NULL) {
		ex_log(LOG_ERROR, "plat_thread_create_ex invalid param");
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->hwin != NULL) {
		ex_log(LOG_ERROR, "plat_thread_create_ex handle->hwin != 0");
		return THREAD_RES_OK;
	}
	
	handle->hwin = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)routine, (LPVOID)&arg->params, 0, NULL);


	if (handle->hwin == NULL) {
		ex_log(LOG_ERROR, "plat_thread_create_ex failed ,error = %d", GetLastError());
		return THREAD_ERR_CREATE_FAILED;
	}
	return THREAD_RES_OK;
}

int plat_thread_create(thread_handle_t* handle, void* routine)
{
	if (handle == NULL || routine == NULL) {
		ex_log(LOG_ERROR, "plat_thread_create invalid param");
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->hwin != NULL) {
		ex_log(LOG_ERROR, "plat_thread_create handle->hwin != 0");
		return THREAD_RES_OK;
	}

	handle->hwin = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)routine, NULL, 0, NULL);
	if (handle->hwin == NULL) {
		ex_log(LOG_ERROR, "CreateThread failed ,error = %d", GetLastError());
		return THREAD_ERR_CREATE_FAILED;
	}
	return THREAD_RES_OK;
}

int plat_thread_release(thread_handle_t* handle)
{
	if (handle == NULL) {
		ex_log(LOG_ERROR, "plat_thread_release handle == NULL");
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->hwin == NULL) {
		ex_log(LOG_ERROR, "plat_thread_release handle->hwin == NULL, thread has been closed");
		return THREAD_RES_OK;
	}

	WaitForSingleObject(handle->hwin, INFINITE);
	CloseHandle(handle->hwin);
	handle->hwin = NULL;
	return THREAD_RES_OK;
}

int plat_mutex_create(mutex_handle_t* handle)
{
	if (handle == NULL) {
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->mutex != NULL) {
		ex_log(LOG_ERROR, "plat_mutex_create handle->mutex != NULL, mutex has already been created");
		return THREAD_RES_OK;
	}
	handle->mutex = CreateMutex(NULL, FALSE, NULL);

	if (handle->mutex == NULL) {
		handle->mutex = NULL;
		return THREAD_ERR_CREATE_FAILED;
	}
	return THREAD_RES_OK;
}

int plat_mutex_release(mutex_handle_t* handle)
{
	if (handle == NULL) {
		ex_log(LOG_ERROR, "plat_mutex_release handle == NULL");
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->mutex == NULL) {
		ex_log(LOG_INFO, "plat_mutex_release handle->mutex == NULL, mutex has already been closed");
		return THREAD_RES_OK;
	}

	//CloseHandle(handle->mutex);
	handle->mutex = NULL;
	return THREAD_RES_OK;
}

int plat_mutex_lock(mutex_handle_t handle)
{
	DWORD retval;
	if (handle.mutex == NULL) {
		return THREAD_ERR_INVALID_PARAM;
	}

	retval = WaitForSingleObject(handle.mutex, INFINITE);

	if (retval == WAIT_OBJECT_0)
		retval = THREAD_RES_OK;
	else
		retval = THREAD_ERR_FAILED;

	return retval;
}

int plat_mutex_trylock(mutex_handle_t handle)
{
	DWORD retval;
	if (handle.mutex == NULL) {
		return THREAD_ERR_INVALID_PARAM;
	}

	retval = WaitForSingleObject(handle.mutex, 0);

	if (retval == WAIT_OBJECT_0) {
		retval = THREAD_RES_OK;
	}
	else if (retval == WAIT_TIMEOUT) {
		retval = THREAD_RES_WAIT_TIMEOUT;
	}
	else {
		retval = THREAD_ERR_FAILED;
	}
	return retval;
}

int plat_mutex_unlock(mutex_handle_t handle)
{
	int retval;
	if (handle.mutex == NULL) {
		return THREAD_ERR_INVALID_PARAM;
	}

	if (!ReleaseMutex(handle.mutex)) {
		ex_log(LOG_ERROR, "ReleaseMutex failed ,error = %d", GetLastError());
		retval = THREAD_ERR_FAILED;
	}
	else
		retval = THREAD_RES_OK;
	return retval;
}

int plat_semaphore_create(semaphore_handle_t* handle, unsigned int initial_cnt, unsigned int max_cnt)
{
	if (handle == NULL || initial_cnt > max_cnt) {
		ex_log(LOG_ERROR, "plat_semaphore_create THREAD_ERR_INVALID_PARAM");
		return THREAD_ERR_INVALID_PARAM;
	}

	if (handle->sema != NULL) {
		ex_log(LOG_DEBUG, "plat_semaphore_create handle->sema != NULL, semaphore has already created");
		return THREAD_RES_OK;
	}

	handle->sema = CreateSemaphore(NULL, initial_cnt, max_cnt, NULL);
	if (handle->sema == NULL) {
		ex_log(LOG_ERROR, "CreateThread failed ,error = %d", GetLastError());
		return THREAD_ERR_CREATE_FAILED;
	}
	return THREAD_RES_OK;
}

int plat_semaphore_release(semaphore_handle_t* handle)
{
	if (handle == NULL)
		return THREAD_ERR_INVALID_PARAM;

	if (handle->sema == NULL) {
		ex_log(LOG_ERROR, "plat_semaphore_release *handle == NULL, no semaphore needs release");
		return THREAD_RES_OK;
	}
	CloseHandle(handle->sema);
	handle->sema = NULL;
	return THREAD_RES_OK;
}

int plat_semaphore_wait(semaphore_handle_t handle, int wait_time)
{
	int retval;
	if (handle.sema == NULL) {
		ex_log(LOG_ERROR, "one method was called before thread manager init");
		return THREAD_RES_OK;
	}
	retval = WaitForSingleObject(handle.sema, wait_time);
	if (retval == WAIT_OBJECT_0) {
		retval = THREAD_RES_OK;
	}
	else if (retval == WAIT_TIMEOUT) {
		retval = THREAD_RES_WAIT_TIMEOUT;
	}
	else {
		retval = THREAD_ERR_FAILED;
	}
	return retval;
}

int plat_semaphore_post(semaphore_handle_t handle)
{
	return ReleaseSemaphore(handle.sema, 1, NULL);
}

int plat_sched_setaffinity_ex(BYTE cpu_mask)
{
	int syscallres = 0;

	/*ex_log(LOG_DEBUG,"plat_sched_setaffinity_ex: %02x", cpu_mask);

	syscallres = syscall(__NR_sched_setaffinity, gettid(), sizeof(cpu_mask),
			     &cpu_mask);
	if (syscallres) {
		ex_log(LOG_ERROR,"Error in the syscall setaffinity: mask = %d, err=%d",
			  cpu_mask, errno);
	}*/

	return syscallres;
}

int plat_sched_setaffinity()
{
	return plat_sched_setaffinity_ex(0xf0);
}

int egistec_clock(){
	return 0;
}

int egistec_cpu_id() {
	return 0;
}
