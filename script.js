(function () {
    var body = document.body;
    var botao = document.getElementById('theme-switch');

    function ler() {
        try { return localStorage.getItem('theme'); } catch (e) { return null; }
    }

    function aplicar(escuro) {
        body.classList.toggle('dark-mode', escuro);
        body.classList.toggle('light-mode', !escuro);
        if (botao) {
            botao.textContent = escuro ? 'Modo claro' : 'Modo escuro';
            botao.setAttribute('aria-pressed', escuro ? 'true' : 'false');
        }
    }

    var salvo = ler();
    var sistemaEscuro = window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
    aplicar(salvo ? salvo === 'dark' : sistemaEscuro);

    if (botao) {
        botao.addEventListener('click', function () {
            var escuro = !body.classList.contains('dark-mode');
            aplicar(escuro);
            try { localStorage.setItem('theme', escuro ? 'dark' : 'light'); } catch (e) {}
        });
    }

    // Mantém as janelas dos programas (iframes) no mesmo tema da página
    window.addEventListener('storage', function (e) {
        if (e.key === 'theme') aplicar(e.newValue === 'dark');
    });
})();

function abrirModal(imagem) {
    var modal = document.getElementById('modal');
    if (!modal) return;
    document.getElementById('imagemModal').src = imagem.src;
    document.getElementById('imagemModal').alt = imagem.alt;
    var legenda = document.getElementById('legendaModal');
    if (legenda) legenda.textContent = imagem.alt;
    modal.style.display = 'block';
}

function fecharModal() {
    var modal = document.getElementById('modal');
    if (modal) modal.style.display = 'none';
}

document.addEventListener('keydown', function (e) {
    if (e.key === 'Escape') fecharModal();
});
