/*
 * Guide4PC 场景组件
 *
 * md 里写 :::scene{id="..."} + :::scene-line{key="..."}，这里负责把它变成
 * 有编排的交互段落。文案留在 md，动画与分支逻辑留在这里。
 */
(function () {
  'use strict';

  var ZH = /^zh/i.test(document.documentElement.lang || '');

  function h(tag, attrs, children) {
    var el = document.createElement(tag);
    if (attrs) {
      Object.keys(attrs).forEach(function (key) {
        var value = attrs[key];
        if (value == null) return;
        if (key === 'class') el.className = value;
        else if (key === 'text') el.textContent = value;
        else el.setAttribute(key, value);
      });
    }
    (children || []).forEach(function (child) {
      if (child == null) return;
      el.appendChild(typeof child === 'string' ? document.createTextNode(child) : child);
    });
    return el;
  }

  function prefersReduced() {
    return !!(window.matchMedia && window.matchMedia('(prefers-reduced-motion: reduce)').matches);
  }

  function readLines(mount) {
    var lines = {};
    mount.querySelectorAll('[data-line]').forEach(function (node) {
      lines[node.getAttribute('data-line')] = (node.textContent || '').trim();
    });
    return lines;
  }

  function blinkTestCta() {
    var link = document.getElementById('test-link');
    if (!link) return;
    link.classList.add('is-blinking');
    link.addEventListener('animationend', function () {
      link.classList.remove('is-blinking');
    }, { once: true });
  }

  // ── welcome-intro ──────────────────────────────────────────────────────
  function buildWelcomeIntro(mount, lines) {
    mount.classList.add('scene-ready');
    mount.textContent = '';

    var helloText = lines.hello || 'hello';
    var section = h('section', { class: 'scene scene-intro' });

    var heroWrap = h('div', { class: 'scene-hero-wrap' });
    var hero = h('div', { class: 'scene-hero' });
    var big = h('span', { class: 'scene-hello-big' });
    helloText.split('').forEach(function (ch, i) {
      var letter = h('span', { class: 'scene-hello-ch', text: ch === ' ' ? '\u00a0' : ch });
      letter.style.setProperty('--i', String(i));
      big.appendChild(letter);
    });
    hero.appendChild(big);
    heroWrap.appendChild(hero);

    var flow = h('div', { class: 'scene-flow' });
    var small = h('p', { class: 'scene-hello-small', text: helloText });
    var action = h('div', { class: 'scene-action' });
    var button = h('button', { class: 'scene-square', type: 'button', 'aria-label': ZH ? '继续' : 'Continue' });
    action.appendChild(button);

    var arrowRow = h('div', { class: 'scene-arrow-row' });
    var arrow = h('button', { class: 'scene-arrow', type: 'button', 'aria-label': ZH ? '继续' : 'Continue' });
    arrow.innerHTML = '<svg viewBox="0 0 44 18" aria-hidden="true"><path d="M1 9h36M31 2.5 38.5 9 31 15.5"/></svg>';
    arrowRow.appendChild(arrow);

    var nowLine = h('p', { class: 'scene-line scene-line-now', text: lines.now || '' });
    var trickLine = h('p', { class: 'scene-line scene-line-trick', text: lines.trick || '' });
    var afterLine = h('p', { class: 'scene-line scene-line-after', text: lines['after-early'] || '' });
    var finalLine = h('p', { class: 'scene-line scene-line-final', text: lines.final || '' });

    flow.append(small, action, arrowRow, nowLine, trickLine, afterLine, finalLine);
    section.append(heroWrap, flow);
    mount.appendChild(section);

    function reveal(node, delay) {
      if (!node || !node.textContent) return;
      window.setTimeout(function () { node.classList.add('is-in'); }, delay || 0);
    }

    var state = { arrowShown: false, proceeded: false, finalShown: false };
    var arrowTimer = 0;
    var armed = false;

    function showArrow() {
      if (state.proceeded) return;
      state.arrowShown = true;
      arrowRow.classList.add('is-in');
    }

    function afterShrink() {
      action.classList.add('is-in');
      arrowTimer = window.setTimeout(showArrow, 2000);
    }

    function proceed(kind) {
      if (state.proceeded) return;
      state.proceeded = true;
      window.clearTimeout(arrowTimer);
      action.classList.add('is-gone');
      arrowRow.classList.add('is-gone');
      reveal(nowLine, 140);
      if (kind === 'arrow') {
        reveal(trickLine, 420);
        reveal(afterLine, 840);
      } else {
        reveal(afterLine, 420);
      }
      window.setTimeout(armAnywhere, 1100);
    }

    function armAnywhere() {
      if (armed) return;
      armed = true;
      document.addEventListener('pointerdown', onAnywhere, true);
    }

    function onAnywhere() {
      if (state.finalShown) return;
      state.finalShown = true;
      document.removeEventListener('pointerdown', onAnywhere, true);
      reveal(finalLine, 120);
      blinkTestCta();
    }

    function shrink() {
      var heroRect = hero.getBoundingClientRect();
      var smallRect = small.getBoundingClientRect();
      if (!heroRect.width || prefersReduced()) {
        heroWrap.remove();
        small.classList.add('is-in');
        afterShrink();
        return;
      }

      var clone = hero.cloneNode(true);
      clone.className = 'scene-hero scene-hero-clone';
      clone.style.position = 'fixed';
      clone.style.left = heroRect.left + 'px';
      clone.style.top = heroRect.top + 'px';
      clone.style.width = heroRect.width + 'px';
      clone.style.margin = '0';
      clone.style.zIndex = '40';
      clone.style.pointerEvents = 'none';
      clone.style.transformOrigin = 'top left';
      document.body.appendChild(clone);

      heroWrap.style.height = heroRect.height + 'px';
      heroWrap.style.overflow = 'hidden';
      hero.style.visibility = 'hidden';
      small.style.visibility = 'hidden';
      var duration = 760;
      var start = performance.now();

      function frame(now) {
        var k = Math.min(1, (now - start) / duration);
        var target = small.getBoundingClientRect();
        var scale = target.height / heroRect.height;
        var dx = target.left - heroRect.left + (target.width - heroRect.width * scale) / 2;
        var dy = target.top - heroRect.top;
        clone.style.transform = 'translate(' + dx + 'px,' + dy + 'px) scale(' + scale + ')';
        if (k < 1) {
          requestAnimationFrame(frame);
        } else {
          clone.remove();
          heroWrap.remove();
          small.style.visibility = 'visible';
          small.classList.add('is-in');
          afterShrink();
        }
      }

      heroWrap.style.transition = 'height ' + duration + 'ms cubic-bezier(.65,0,.35,1)';
      heroWrap.style.height = '0px';
      requestAnimationFrame(frame);
    }

    button.addEventListener('click', function (e) { e.preventDefault(); proceed('button'); });
    arrow.addEventListener('click', function (e) { e.preventDefault(); proceed('arrow'); });

    if (prefersReduced()) {
      heroWrap.remove();
      small.classList.add('is-in');
      afterShrink();
    } else {
      window.setTimeout(shrink, 1450);
    }
  }

  function hydrateAll() {
    document.querySelectorAll('.scene-mount').forEach(function (mount) {
      if (mount.getAttribute('data-ready') === '1') return;
      mount.setAttribute('data-ready', '1');
      var lines = readLines(mount);
      if (mount.getAttribute('data-scene') === 'welcome-intro') {
        buildWelcomeIntro(mount, lines);
      }
    });
  }

  window.G4PCScene = { hydrateAll: hydrateAll };
})();
