Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/focal64"
  config.vm.box_version = "20240821.0.1"
    
  config.vm.provision "shell", inline: <<-SHELL
    useradd -m -s /bin/bash -p $(openssl passwd -1 test) test
    echo 'test ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/test
    sed -i 's/^PasswordAuthentication no/PasswordAuthentication yes/' /etc/ssh/sshd_config
    sed -i 's/^#PasswordAuthentication yes/PasswordAuthentication yes/' /etc/ssh/sshd_config
    sed -i 's/^ChallengeResponseAuthentication no/ChallengeResponseAuthentication yes/' /etc/ssh/sshd_config
    sed -i 's/^PubkeyAuthentication yes/PubkeyAuthentication no/' /etc/ssh/sshd_config
    systemctl restart sshd

    apt-get update
    apt-get install -y python2 python2-dev g++ git
    curl -O https://bootstrap.pypa.io/pip/2.7/get-pip.py
    python2 get-pip.py
    rm get-pip.py
    pip2 install pytest tox
    update-alternatives --install /usr/bin/python python /usr/bin/python2 1

    cd /home/test
    git clone https://github.com/DudkoAndrei/lesta-ai-task.git
    cd lesta-ai-task
    mkdir .build && cd .build
    g++ -shared -fPIC -o matrix_lib.so ../src/matrix_wrapper.cpp $(python2-config --includes --ldflags) -Wno-write-strings -std=c++11
    cp ../src/matrix_mul.py .
    echo "export PYTHONPATH=$(pwd):$PYTHONPATH" >> /home/test/.bashrc
    cd /home/test
    cp lesta-ai-task/tox.ini .
    mv lesta-ai-task/tests/ ./
    cp lesta-ai-task/src/matrix_mul.py .
    cp -a lesta-ai-task/example_data/. ./
    chown test -R /home/test
  SHELL
end
