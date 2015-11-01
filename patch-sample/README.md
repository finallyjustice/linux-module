This is a tutorial on how to make and email out the patch for Linux kernel.

1. Download the Linux kernel code, e.g., the master branch of linux-stable:

Website: http://git.kernel.org/cgit/linux/kernel/git/stable/linux-stable.git

# git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git

2. Configure your git information:

# git config --add user.name "FirstName LastName"
# git config --add user.email "user@gmail.com"

3. Suppose there is a bug to fix. Create and switch to a new branch:

# git branch MyBugFixPoint
# git checkout MyBugFixPoint

4. Change the code of kernel, e.g., add printk in init/main.c.

5. Commit the change:

# git commit -a

6. Generate the patch with git:

# git format-patch -s -n master..MyBugFixPoint

7. Check your patch for errors

# scripts/checkpatch.pl 0001-patch-name

8. Obtain the list of corresponding maintainers:

# scripts/get_maintainer.pl 0001-patch-name

9. Email out the patch:

# git send-email --smtp-encryption=tls --smtp-server=smtp.gmail.com \
# --smtp-user=user@gmail.com --smtp-server-port=587 \
# --from "FirstName LastName" --to "maintainer1@gmail.com" \
# --to "maintainer2@gmail.com" --cc "maintainer3@hotmail.com" \
# 0001-patch-name


Discussion: 

There is another way to email out the patch with msmtp utility.

Please copy msmtprc to ~/.msmtprc and change the corresponding fields.

# git send-email --smtp-server /usr/bin/msmtp --to maintainer@hotmail.com \
# 0001-patch-name

How to apply a patch:

(1) 
# patch -p1 < 0001-patch-name
# git commit -a 

(2) 
# git am 0001-patch-name

