let toggle = true;
const handleGarage = () => {
  console.log('clicou');
  toggle = !toggle;
  console.log(toggle);
  const mae = document.getElementById('mae').innerHTML;
  mae === 'Controle de locais'
    ? (document.getElementById('mae').innerHTML = 'Não tá mais')
    : (document.getElementById('mae').innerHTML = 'Controle de locais');
  console.log(mae);
};

setInterval(() => {
  const date = new Date();
  const newDate = new Intl.DateTimeFormat('br-FR', {
    year: 'numeric',
    month: 'numeric',
    day: 'numeric',
    hour: 'numeric',
    minute: 'numeric',
    second: 'numeric',
  }).format(date);

  document.getElementById('relogio').innerHTML = newDate;
}, 1000);

// setInterval(() => {
//   const root = document.getElementById('root');
//   console.log(root.style.background);
//   root.style.background === 'rgb(51, 76, 255)'
//     ? (root.style.background = 'rgb(255, 87, 51)')
//     : (root.style.background = 'rgb(51, 76, 255)');
// }, 1000);
