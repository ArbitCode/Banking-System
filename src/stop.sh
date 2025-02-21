#!/bin/bash

APP_NAME="bank_backend"
SERVICE_FILE="/etc/systemd/system/${APP_NAME}.service"

echo "Stopping bank_backend service..."
sudo systemctl stop "$APP_NAME"
sudo systemctl disable "$APP_NAME"
sudo rm -f "$SERVICE_FILE"
sudo systemctl daemon-reload

echo "Service stopped and removed."

