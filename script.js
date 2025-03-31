document.addEventListener("DOMContentLoaded", () => {
    const themeSwitch = document.getElementById('theme-switch');
    const body = document.body;

    // Verifica se existe um tema salvo no localStorage
    const savedTheme = localStorage.getItem('theme');
    if (savedTheme === 'dark') {
        body.classList.add('dark-mode');
        body.classList.remove('light-mode');
        themeSwitch.checked = true;
    } else {
        body.classList.add('light-mode');
        body.classList.remove('dark-mode');
        themeSwitch.checked = false;
    }

    // Alterna o tema ao clicar no switch
    themeSwitch.addEventListener('change', () => {
        if (themeSwitch.checked) {
            body.classList.add('dark-mode');
            body.classList.remove('light-mode');
            localStorage.setItem('theme', 'dark');
        } else {
            body.classList.add('light-mode');
            body.classList.remove('dark-mode');
            localStorage.setItem('theme', 'light');
        }
    });
});

function abrirModal(imagem) {
    var modal = document.getElementById("modal");
    var modalImg = document.getElementById("imagemModal");
    modal.style.display = "block";
    modalImg.src = imagem.src;
}

function fecharModal() {
    document.getElementById("modal").style.display = "none";
}
