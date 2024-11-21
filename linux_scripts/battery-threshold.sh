#!/bin/bash

# Help function to display usage
show_help() {
    echo "Usage: $0 -<value>"
    echo "Set battery charge threshold (0-100)"
    echo "Examples:"
    echo "  $0 -60   # Set charge threshold to 60%"
    echo "  $0 -80   # Set charge threshold to 80%"
        # Check and display the power status
    AC_STATUS_FILE="/sys/class/power_supply/ACAD/online"
    if [ -f "$AC_STATUS_FILE" ]; then
        AC_STATUS=$(cat "$AC_STATUS_FILE")
        if [ "$AC_STATUS" -eq 1 ]; then
            echo "Power Status: Connected to AC adapter (1)"
        else
            echo "Power Status: Running on battery (0)"
        fi
    else
        echo "Power Status: Unable to determine (file not found: $AC_STATUS_FILE)"
    fi
    exit 1
}

# Check if argument is passed
if [ -z "$1" ] || [ $# -ne 1 ]; then
    show_help
fi

# Extract threshold value from argument (e.g., -60 -> 60)
THRESHOLD=$(echo "$1" | tr -d '-')

# Validate threshold value is a valid number between 0 and 100
if ! [[ "$THRESHOLD" =~ ^[0-9]+$ ]] || [ "$THRESHOLD" -lt 0 ] || [ "$THRESHOLD" -gt 100 ]; then
    echo "Error: Invalid threshold value. Please provide a number between 0 and 100."
    exit 1
fi

# Check if script is run with sudo/root privileges
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root or with sudo"
    exit 1
fi

# Verify battery file exists
BATTERY_FILE="/sys/class/power_supply/BAT1/charge_control_end_threshold"
if [ ! -f "$BATTERY_FILE" ]; then
    echo "Error: Battery threshold control file not found. Check your system configuration."
    exit 1
fi

# Set the threshold dynamically
echo "$THRESHOLD" > "$BATTERY_FILE"

# Prepare cron job for persistent setting
CRON_JOB="@reboot root echo $THRESHOLD > $BATTERY_FILE"

# Remove any existing similar cron job and add new one
(crontab -l 2>/dev/null | grep -v "charge_control_end_threshold" ; echo "$CRON_JOB") | crontab -

# Create a systemd service for additional reliability
SERVICE_FILE="/etc/systemd/system/battery-threshold.service"
cat > "$SERVICE_FILE" << EOL
[Unit]
Description=Set Battery Charge Threshold
After=battery.target

[Service]
Type=oneshot
ExecStart=/bin/bash -c 'echo $THRESHOLD > $BATTERY_FILE'

[Install]
WantedBy=multi-user.target
EOL

# Enable and start the systemd service
systemctl daemon-reload
systemctl enable battery-threshold.service
systemctl restart battery-threshold.service

echo "Battery charge threshold successfully set to $THRESHOLD%"
echo "Threshold will persist across reboots via crontab and systemd service"
