#include "statusbar.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BATTERY_PATH "/sys/class/power_supply/BAT0/"

static int capacity_fd = -1;
static int status_fd = -1;

size_t battery_update(char * output, size_t length)
{
	char status[16];
	char capacity[4];
	const char *icon = "";

	ssize_t read_len = 0;
	size_t len = 0;

	capacity_fd = open(BATTERY_PATH "capacity", O_RDONLY);
	if (capacity_fd == -1)
	{
		perror("could not open battery capacity info");
		goto exit;
	}

	status_fd = open(BATTERY_PATH "status", O_RDONLY);
	if (status_fd == -1)
	{
		perror("could not open battery status info");
		goto exit;
	}

	read_len = read(capacity_fd, capacity, 4);
	if (read_len <= 0)
	{
		goto exit;
	}

	capacity[read_len-1] = '\0';

	read_len = read(status_fd, status, 16);
	if (read_len <= 0)
	{
		goto exit;
	}

	status[read_len-1] = '\0';

	if (strcmp(status, "Full") == 0)
	{
		goto exit;
	}
	else if (strcmp(status, "Charging") == 0)
	{
		if (atoi(capacity) >= 90)
		{
			goto exit;
		}

		icon = "\u26a1";
	}

	len = snprintf(output, length, "Bat:%s%s%%", icon, capacity);

exit:
	close(capacity_fd);
	close(status_fd);
	
	capacity_fd = -1;
	status_fd = -1;

	return len;
}

