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

# Check for multiple possible battery paths
BATTERY_PATHS=(
    "/sys/class/power_supply/BAT0/charge_control_end_threshold"
    "/sys/class/power_supply/BAT1/charge_control_end_threshold"
    "/sys/class/power_supply/BAT0/charge_stop_threshold"
    "/sys/class/power_supply/BAT1/charge_stop_threshold"
)

BATTERY_FILE=""
for path in "${BATTERY_PATHS[@]}"; do
    if [ -f "$path" ]; then
        BATTERY_FILE="$path"
        break
    fi
done

if [ -z "$BATTERY_FILE" ]; then
    echo "Error: No battery threshold control file found. Check your system configuration."
    exit 1
fi

# Create a udev rule for persistent setting
UDEV_RULE_FILE="/etc/udev/rules.d/99-battery-threshold.rules"
cat > "$UDEV_RULE_FILE" << EOL
SUBSYSTEM=="power_supply", ATTR{status}=="Charging", RUN+="/bin/bash -c 'echo $THRESHOLD > $BATTERY_FILE'"
EOL

# Create a systemd service
SERVICE_FILE="/etc/systemd/system/battery-threshold.service"
cat > "$SERVICE_FILE" << EOL
[Unit]
Description=Set Battery Charge Threshold
After=multi-user.target
StartLimitIntervalSec=0

[Service]
Type=oneshot
RemainAfterExit=yes
ExecStart=/bin/bash -c 'echo $THRESHOLD > $BATTERY_FILE'
Restart=on-failure
RestartSec=1s

[Install]
WantedBy=multi-user.target
EOL

# Set the threshold immediately
echo "$THRESHOLD" > "$BATTERY_FILE"

# Reload udev rules
udevadm control --reload-rules
udevadm trigger

# Enable and start the systemd service
systemctl daemon-reload
systemctl enable battery-threshold.service
systemctl restart battery-threshold.service

# Create a startup script in rc.local
RC_LOCAL="/etc/rc.local"
if [ ! -f "$RC_LOCAL" ]; then
    echo '#!/bin/bash' > "$RC_LOCAL"
    chmod +x "$RC_LOCAL"
fi

# Remove any existing battery threshold commands
sed -i '/charge_control_end_threshold/d' "$RC_LOCAL"
sed -i '/charge_stop_threshold/d' "$RC_LOCAL"

# Add new command before 'exit 0' or at the end
if grep -q "exit 0" "$RC_LOCAL"; then
    sed -i "/exit 0/i echo $THRESHOLD > $BATTERY_FILE" "$RC_LOCAL"
else
    echo "echo $THRESHOLD > $BATTERY_FILE" >> "$RC_LOCAL"
    echo "exit 0" >> "$RC_LOCAL"
fi

echo "Battery charge threshold successfully set to $THRESHOLD%"
echo "Threshold will persist across reboots via multiple methods:"
echo "1. udev rule"
echo "2. systemd service"
echo "3. rc.local script"
echo "Current battery threshold file: $BATTERY_FILE"
