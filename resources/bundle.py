import sys
import subprocess


def main():
    prog_name, *argv = sys.argv
    output, *argv = argv
    resources = argv
    text = ""
    for f in resources:
        try:
            out = subprocess.run(
                ["xxd", "-i", f],
                capture_output=True,
                check=True,
                text=True,
            )
        except subprocess.CalledProcessError as e:
            print(
                "xxd exited with code",
                e.returncode,
                ":",
                e.stderr,
                file=sys.stderr,
            )
            exit(1)
        text += out.stdout
        text += "\n"
    with open(output, "w") as file:
        file.write(text)


if __name__ == "__main__":
    main()
