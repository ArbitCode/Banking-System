import subprocess
import sys

PACKAGE_MAP = {
    "unixodbc": {
        "dnf": "unixODBC",
        "yum": "unixODBC",
        "apt": "unixodbc"
    },
    "unixodbc-devel": {
        "dnf": "unixODBC-devel",
        "yum": "unixODBC-devel",
        "apt": "unixodbc-dev"
    },
    "postgresql-odbc": {
        "dnf": "postgresql-odbc",
        "yum": "postgresql-odbc",
        "apt": "odbc-postgresql"
    },
    "gcc-c++": {
        "dnf": "gcc-c++",
        "yum": "gcc-c++",
        "apt": "g++"
    },
    "make": {
        "dnf": "make",
        "yum": "make",
        "apt": "make"
    },
    "cmake": {
        "dnf": "cmake",
        "yum": "cmake",
        "apt": "cmake"
    },
    "git": {
        "dnf": "git",
        "yum": "git",
        "apt": "git"
    }
}

def detect_package_manager():
    """Detect the system's package manager (dnf, yum, or apt)."""
    try:
        subprocess.run(["dnf", "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return "dnf"
    except FileNotFoundError:
        try:
            subprocess.run(["yum", "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            return "yum"
        except FileNotFoundError:
            try:
                subprocess.run(["apt", "--version"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                return "apt"
            except FileNotFoundError:
                sys.exit("Unsupported package manager. Only dnf, yum, or apt are supported.")

def install_packages(package_manager, packages):
    """Install packages using the detected package manager."""
    try:
        if package_manager in ["dnf", "yum"]:
            subprocess.run(["sudo", package_manager, "install", "-y"] + packages, check=True)
        elif package_manager == "apt":
            subprocess.run(["sudo", "apt", "update"], check=True)
            subprocess.run(["sudo", "apt", "install", "-y"] + packages, check=True)
        print("Packages installed successfully!")
    except subprocess.CalledProcessError as e:
        sys.exit(f"Error installing packages: {e}")

if __name__ == "__main__":
    desired_packages = [
        "unixodbc",
        "unixodbc-devel",
        "postgresql-odbc",
        "gcc-c++",
        "make",
        "cmake",
        "git"
    ]

    pm = detect_package_manager()
    system_specific_packages = [PACKAGE_MAP[pkg][pm] for pkg in desired_packages]

    install_packages(pm, system_specific_packages)
