---
layout: default
title: Blog
---


# Blog posts

{% for post in site.posts %}
  {% assign currentdate = post.date | date: "%Y" %}
  {% if currentdate != date %}
    {% unless forloop.first %}</ul>{% endunless %}
## {{ currentdate }}
<ul>
    {% assign date = currentdate %}
  {% endif %}
<li>
<h3><a href="{{ post.url }}">{{ post.title }}</a></h3>
<h4>{{ post.date | date_to_string }}</h4>
<p>{{ post.excerpt }}</p>
</li>
  {% if forloop.last %}</ul>{% endif %}
{% endfor %}

