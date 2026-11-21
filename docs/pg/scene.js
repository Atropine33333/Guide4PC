/*
 * Guide4PC 场景组件
 *
 * md 里写 ::::scene{id="..."} + :::scene-line{key="..."}，这里负责把文案
 * 变成有编排的交互段落。开场 hello 的笔迹参考 CodePen「SVG Stroke
 * Animation」(gau/ZWZKxO)：路径用 stroke-dashoffset 画出来，再整体缩放
 * 到左上角的小 hello。文案留在 md，动画与分支逻辑留在这里。
 */
(function () {
  'use strict';

  var ZH = /^zh/i.test(document.documentElement.lang || '');

  var HELLO_SVG =
    '<svg viewBox="0 0 582 197" xmlns="http://www.w3.org/2000/svg" aria-hidden="true" focusable="false">' +
    '<path class="path path-1" d="M208,338c38-16.67,73.74-45.72,97.33-66,21.33-18.33,32.67-35.67,37.33-52.67C347.12,203.12,344,192,332,192c-11,0-21,10.33-24.94,27.68-4.52,19.89-22.06,107.82-29.39,149,15.67-72.33,36.33-81.33,53.67-81.33,22.33,0,24.67,18.67,19.42,39-5.43,21.07-7.42,44.32,17.91,44.32,18,0,35.53-8.17,52.67-20,14-9.67,23-24,23-40,0-13.42-8-23.33-20.67-23.33s-24.33,12-24.33,33.33c0,27,16.33,48,44,48,25.67,0,47.67-19.67,62-44.67,13.61-23.74,30.67-64.67,33.33-92.67s-5.33-36-18.67-36-24.67,17.33-28.67,43.33S486,302,491.33,330s28,37.67,46,37.67,38.17-15.67,52-37c16.54-25.51,35.87-67.45,38.67-102,2-24.67-8.67-33.33-20-33.33-14.67,0-23.33,13.33-28,38-4.5,23.81-8,64-2,94,4.64,23.21,25.33,40.33,44.67,40.33s32.67-19,36.67-42.33" transform="translate(-199 -183)"/>' +
    '<path class="path path-2" d="M697.33,287.33C672,287.33,661.33,305,659,327c-2.81,26.54,10.33,41.67,29.67,41.67,22,0,34.54-20.78,36.67-40.67,2-18.67-7.39-39.13-28-40.67" transform="translate(-199 -183)"/>' +
    '<path class="path path-3" d="M714.8,295.12c12.11,12.26,43.53,9.55,56.53-5.79" transform="translate(-199 -183)"/>' +
    '<line class="path path-4" x1="561" y1="181.67" x2="561" y2="181.67"/>' +
    '</svg>';

  var DRAW_MS = 6150;   // 笔迹画完（CodePen 的 7.6s × 0.8）
  var PAUSE_MS = 260;
  var SHRINK_MS = 900;

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
    window.setTimeout(function () {
      link.classList.remove('is-blinking');
    }, 3600);
  }

  var activeCleanup = null;

  // ── welcome-intro ──────────────────────────────────────────────────────
  function buildWelcomeIntro(mount, lines) {
    if (activeCleanup) activeCleanup();
    mount.classList.add('scene-ready');
    mount.textContent = '';

    var timers = [];
    var listeners = [];
    var liveClone = null;
    var finished = false;

    function later(fn, ms) {
      var id = window.setTimeout(fn, ms);
      timers.push(id);
      return id;
    }
    function listen(target, type, fn, opts) {
      target.addEventListener(type, fn, opts);
      listeners.push(function () { target.removeEventListener(type, fn, opts); });
    }
    function cleanup() {
      timers.forEach(function (id) { window.clearTimeout(id); });
      timers.length = 0;
      listeners.forEach(function (off) { off(); });
      listeners.length = 0;
      if (liveClone && liveClone.parentNode) liveClone.parentNode.removeChild(liveClone);
      liveClone = null;
      activeCleanup = null;
    }
    activeCleanup = cleanup;

    var section = h('section', { class: 'scene scene-intro' });

    // 舞台铺满开场区域；hello 在其中水平垂直居中。
    var stage = h('div', { class: 'scene-stage' });
    var draw = h('div', { class: 'scene-draw' });
    draw.innerHTML = HELLO_SVG;
    stage.appendChild(draw);

    var flow = h('div', { class: 'scene-flow' });
    var small = h('span', { class: 'scene-hello-small' });
    small.innerHTML = HELLO_SVG;

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
    section.append(stage, flow);
    mount.appendChild(section);

    button.disabled = true;
    arrow.disabled = true;

    var state = { arrowShown: false, proceeded: false, finalShown: false };
    var arrowTimer = 0;

    function reveal(node, delay) {
      if (!node || !node.textContent) return;
      later(function () { node.classList.add('is-in'); }, delay || 0);
    }

    function showArrow() {
      if (state.proceeded) return;
      state.arrowShown = true;
      arrow.disabled = false;
      arrowRow.classList.add('is-in');
    }

    function afterShrink() {
      finished = true;
      small.classList.add('is-in');
      stage.remove();
      button.disabled = false;
      action.classList.add('is-in');
      arrowTimer = later(showArrow, 2000);
    }

    function freezeStrokes(el) {
      el.querySelectorAll('.path').forEach(function (p) {
        var cs = window.getComputedStyle(p);
        p.style.strokeDasharray = cs.strokeDasharray;
        p.style.strokeDashoffset = '0';
        p.style.strokeWidth = cs.strokeWidth;
        p.style.transition = 'none';
      });
    }

    function shrink() {
      var rect = draw.getBoundingClientRect();
      var target = small.getBoundingClientRect();
      if (!rect.width || !target.width || prefersReduced()) {
        afterShrink();
        return;
      }

      var clone = draw.cloneNode(true);
      freezeStrokes(clone);
      clone.classList.add('scene-draw-clone');
      clone.style.position = 'fixed';
      clone.style.left = rect.left + 'px';
      clone.style.top = rect.top + 'px';
      clone.style.width = rect.width + 'px';
      clone.style.height = rect.height + 'px';
      clone.style.margin = '0';
      clone.style.zIndex = '40';
      clone.style.pointerEvents = 'none';
      clone.style.transformOrigin = 'top left';
      clone.style.transition = 'transform ' + SHRINK_MS + 'ms cubic-bezier(.65,0,.35,1)';
      document.body.appendChild(clone);
      liveClone = clone;
      // 只留克隆体在动：原图立刻藏起来，避免出现两个 hello 叠在一起。
      draw.style.visibility = 'hidden';

      var scale = target.height / rect.height;
      var dx = target.left - rect.left;
      var dy = target.top - rect.top;

      requestAnimationFrame(function () {
        requestAnimationFrame(function () {
          clone.style.transform = 'translate(' + dx + 'px,' + dy + 'px) scale(' + scale + ')';
        });
      });

      later(function () {
        if (liveClone === clone) {
          clone.remove();
          liveClone = null;
        }
        afterShrink();
      }, SHRINK_MS + 80);
    }

    function proceed(kind) {
      if (state.proceeded) return;
      state.proceeded = true;
      window.clearTimeout(arrowTimer);
      button.disabled = true;
      arrow.disabled = true;
      action.classList.add('is-gone');
      arrowRow.classList.add('is-gone');
      reveal(nowLine, 140);
      if (kind === 'arrow') {
        reveal(trickLine, 420);
        reveal(afterLine, 840);
      } else {
        reveal(afterLine, 420);
      }
      later(armAnywhere, 1100);
    }

    var armed = false;
    function armAnywhere() {
      if (armed) return;
      armed = true;
      listen(document, 'pointerdown', onAnywhere, true);
      listen(document, 'keydown', onAnywhere, true);
    }

    function onAnywhere() {
      if (state.finalShown) return;
      state.finalShown = true;
      document.removeEventListener('pointerdown', onAnywhere, true);
      document.removeEventListener('keydown', onAnywhere, true);
      reveal(finalLine, 120);
      blinkTestCta();
    }

    button.addEventListener('click', function (e) { e.preventDefault(); proceed('button'); });
    arrow.addEventListener('click', function (e) { e.preventDefault(); proceed('arrow'); });

    if (prefersReduced()) {
      afterShrink();
    } else {
      // 触发 CSS 里的 stroke 过渡；画完后再收缩。
      later(function () { draw.classList.add('is-drawing'); }, 60);
      later(shrink, 60 + DRAW_MS + PAUSE_MS);
    }
  }

  function hydrateAll() {
    document.querySelectorAll('.scene-mount').forEach(function (mount) {
      if (mount.getAttribute('data-ready') === '1') return;
      mount.setAttribute('data-ready', '1');
      var lines = readLines(mount);
      if (mount.getAttribute('data-scene') === 'welcome-intro') {
        buildWelcomeIntro(mount, lines);
      } else {
        mount.classList.add('scene-plain');
      }
    });
  }

  window.G4PCScene = {
    hydrateAll: hydrateAll,
    dispose: function () {
      if (activeCleanup) activeCleanup();
    },
  };
})();
