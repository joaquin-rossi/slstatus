/* interval between updates (in ms) */
const unsigned int interval = 100;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define STATUSLEN 2048

/*
 * function             description                     argument (example)
 
 * battery_perc         battery percentage              battery name (BAT0)
 * battery_remaining    battery remaining HH:MM         battery name (BAT0)
 * battery_state        battery charging state          battery name (BAT0)

 * cpu_perc             cpu usage in percent            NULL
 * cpu_freq             cpu frequency in MHz            NULL

 * datetime             date and time                   format string (%F %T)

 * disk_free            free disk space in GB           mountpoint path (/)
 * disk_perc            disk usage in percent           mountpoint path (/)
 * disk_total           total disk space in GB          mountpoint path (/)
 * disk_used            used disk space in GB           mountpoint path (/)

 * echo                 string to echo                  NULL

 * entropy              available entropy               NULL

 * hostname             hostname                        NULL

 * ipv4                 IPv4 address                    interface name (eth0)
 * ipv6                 IPv6 address                    interface name (eth0)

 * kernel_release       `uname -r`                      NULL

 * keyboard_indicators  caps/num lock indicators        format string (c?n?) (see keyboard_indicators.c)
 * keymap               layout (variant) of current     NULL

 * load_avg             load average                    NULL

 * netspeed_rx          receive network speed           interface name (wlan0)
 * netspeed_tx          transfer network speed          interface name (wlan0)

 * ram_free             free memory in GB               NULL
 * ram_perc             memory usage in percent         NULL
 * ram_total            total memory size in GB         NULL
 * ram_used             used memory in GB               NULL

 * num_files            number of files in a directory  path (/home/foo/inbox/cur)

 * run_command          custom shell command            command (echo foo)

 * swap_free            free swap in GB                 NULL
 * swap_perc            swap usage in percent           NULL
 * swap_total           total swap size in GB           NULL
 * swap_used            used swap in GB                 NULL

 * temp                 temperature in degree celsius   sensor file (/sys/class/thermal/...)

 * uptime               system uptime                   NULL

 * gid                  GID of current user             NULL
 * uid                  UID of current user             NULL
 * username             username of current user        NULL

 * volume_perc          OSS/ALSA volume in percent      mixer file (/dev/mixer)

 * wifi_essid           WiFi ESSID                      interface name (wlan0)
 * wifi_perc            WiFi signal in percent          interface name (wlan0)

 * motd                 contents of /etc/motd          NULL
 */

static const char fmt[] = "%$ | ipv4: %$ | volume: %$%% | %$";
static const action_t actions[] = {
	{ .interval = 1000, .signo = 0, .func = keyboard_indicators, .arg = "cn" },
	{ .interval = 1000, .signo = 0, .func = ipv4,                .arg = "enp5s0" },
	{ .interval = 0,    .signo = 1, .func = run_command,         .arg = "pamixer --get-volume" },
	{ .interval = 1000, .signo = 0, .func = datetime,            .arg = "%F %T" },
};
